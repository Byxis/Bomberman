#include "PlayerControl.h"
#include "PlayerHUD.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"
#include "GameFramework/SpringArmComponent.h"
#include "./Bomberman/Bomb/BombHandler.h"
#include <Bomberman/Game/CustomGameMode.h>
#include <Bomberman/Game/CustomGameInstance.h>
#include <Bomberman/HUD/GameoverHUD.h>
#include <Bomberman/Enemy/EnemyHandler.h>

APlayerControl::APlayerControl()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(45, 45);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);

	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	SpringArmComp->bUsePawnControlRotation = false;
	
	GetComponentByClass<UCapsuleComponent>()->OnComponentBeginOverlap.AddDynamic(this, &APlayerControl::OnInteract);
}

void APlayerControl::BeginPlay()
{
	Super::BeginPlay();

	m_gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
	m_gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	m_maxPlacedBomb = m_gameInstance->GetBombLimit();

	if (m_gameMode == nullptr)
		return;
	if (m_gameMode->GetCurrentGameState() == EGameState::Menu)
	{
		OpenMainMenu();
	}
	else if (m_gameMode->GetCurrentGameState() == EGameState::Playing)
	{
		m_playerHud = CreateWidget<UPlayerHUD>(GetGameInstance(), m_playerHudClass, FName("PlayerWidget"));
		if (m_playerHud != nullptr)
		{
			m_playerHud->AddToViewport();
		}
		ActualiseLife();
		ActualiseScore();
	}
}

void APlayerControl::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (m_playerHud != nullptr)
	{
		m_playerHud->RemoveFromParent();
		m_playerHud = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

void APlayerControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ActualiseTimer();

	if (m_isRespawning)
	{
		if (m_respawningTimer > 0)
			m_respawningTimer -= DeltaTime;
		else if (m_gameMode != nullptr)
			m_gameMode->RestartLevel();
		else
			Damage();
			
	}

	SpringArmComp->SetWorldLocationAndRotation(FVector(1700.0f, 900.0f, 1500.0f), FRotator(-90.0f, 0.0f, -90.0f));
}

void APlayerControl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerControl::MoveForward);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerControl::MoveRight);
	PlayerInputComponent->BindAction("Action1", IE_Pressed, this, &APlayerControl::SpawnBomb);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerControl::PauseGame);
	PlayerInputComponent->BindAction("Action2", IE_Pressed, this, &APlayerControl::Detonatebomb);
}

void APlayerControl::MoveForward(float _axis)
{
	if (!m_dead)
	{
		if (_axis > 0.2)
			_axis = 1;
		else if (_axis < -0.2)
			_axis = -1;
		else
			_axis = 0;
		_axis *= m_gameInstance->GetSpeed();
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, _axis *0.8f);
	}
}

void APlayerControl::MoveRight(float _axis)
{
	if (!m_dead)
	{
		if (_axis > 0.2)
			_axis = 1;
		else if (_axis < -0.2)
			_axis = -1;
		else
			_axis = 0;
		_axis *= m_gameInstance->GetSpeed();
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, _axis *-0.8f);
	}
}

void APlayerControl::SpawnBomb()
{
	if (m_maxPlacedBomb > 0 && m_canPlaceBomb && !m_dead)
	{
		const FVector location(FGenericPlatformMath::RoundToInt((GetActorLocation().X - 50) / 100) * 100 + 50,
			FGenericPlatformMath::RoundToInt((GetActorLocation().Y - 50) / 100) * 100 + 50,
			50);
		const FRotator rotation = GetActorRotation();

		if (!hasPlaceToSpawnBomb())
			return;

		FString message = FString::Printf(TEXT("Location: X=%f, Y=%f, Z=%f, remaining:%f"), location.X, location.Y, location.Z, m_maxPlacedBomb);
		AActor* spawnedbomb = GetWorld()->SpawnActor<AActor>(m_bomb, location, rotation);
		if (spawnedbomb != nullptr)
		{
			SetPlacingBomb(true);
			SetCanPlaceBomb(false);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, message);
			UBombHandler* bombHandler = spawnedbomb->GetComponentByClass<UBombHandler>();
			if (bombHandler != nullptr)
				bombHandler->SetPower(m_gameInstance->GetBombPower());

			m_maxPlacedBomb--;
		}
	}
}
void APlayerControl::Detonatebomb()
{
	if (m_gameInstance->HasDetonatorBonus() && !m_dead)
	{
		if (m_gameMode != nullptr)
		{
			for (UBombHandler* bomb : m_gameMode->GetAllBombs())
			{
				bomb->Explode();
			}
		}
	}
}

void APlayerControl::GiveBackBomb()
{
	m_maxPlacedBomb++;
}

void APlayerControl::SetCanPlaceBomb(bool _bool)
{
	m_canPlaceBomb = _bool;
}

void APlayerControl::Damage()
{
	if (!m_dead)
	{
		m_dead = true;
		if (m_gameInstance != nullptr && m_gameInstance->GetLife() > 0)
		{
			m_gameInstance->AddLife(-1);
			ActualiseLife();
			m_isRespawning = true;
			m_respawningTimer = 5.0f;
		}
		else
		{
			m_gameOverHud = CreateWidget<UGameoverHUD>(GetGameInstance(), m_gameOverHudClass, FName("GameoverWidget"));

			m_playerHud->RemoveFromParent();

			if (m_gameOverHud != nullptr)
			{
				m_gameOverHud->AddToViewport();
				GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			}
		}
	}
	
}


void APlayerControl::BonusBombPower()
{
	if (m_gameInstance != nullptr)
	{
		m_gameInstance->AddBombPower(1);
		m_gameInstance->AddScore(1000);
	}
}

void APlayerControl::BonusBombLimit()
{
	if (m_gameInstance != nullptr)
	{
		m_gameInstance->AddBombLimit(1);
		m_gameInstance->AddScore(1000);
		m_maxPlacedBomb++;
	}
}

void APlayerControl::BonusSpeed()
{
	if (m_gameInstance != nullptr)
	{
		m_gameInstance->AddSpeed(0.5f);
		m_gameInstance->AddScore(1000);
	}
}

void APlayerControl::BonusDetonator()
{
	if (m_gameInstance != nullptr)
	{
		m_gameInstance->SetDetonatorBonus(true);
		m_gameInstance->AddScore(1000);
	}
}

void APlayerControl::BonusVest()
{
	if (m_gameInstance != nullptr)
	{
		m_gameInstance->SetVestBonus(true);
		m_gameInstance->AddScore(1000);
	}
}

void APlayerControl::BonusGhostWalls()
{
	if (m_gameInstance != nullptr && m_gameMode != nullptr)
	{
		m_gameInstance->SetGhostWallsBonus(true);
		m_gameMode->SetActiveWallsGhost();
		m_gameInstance->AddScore(1000);
	}
}

void APlayerControl::ActualiseScore()
{
	if (m_gameInstance != nullptr)
	{
		if (m_playerHud != nullptr)
		{
			m_playerHud->SetScore(m_gameInstance->GetScore());
		}
		if (m_pauseHud != nullptr)
		{
			m_pauseHud->SetScore(m_gameInstance->GetScore());
		}
	}
}

void APlayerControl::ActualiseTimer()
{
	if (m_gameMode != nullptr)
	{
		if (m_playerHud != nullptr)
		{
			m_playerHud->SetTime(m_gameMode->GetTimer());
		}
	}
}

void APlayerControl::ActualiseLife()
{
	if (m_gameInstance != nullptr)
	{
		if (m_playerHud != nullptr)
		{
			m_playerHud->SetLife(m_gameInstance->GetLife());
		}
	}
}

void APlayerControl::PauseGame()
{
	m_gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
	if (m_gameMode != nullptr && m_playerHudClass != nullptr && m_pauseHudClass != nullptr)
	{
		if (m_gameMode->GetCurrentGameState() == EGameState::Playing)
		{
			UE_LOG(LogTemp, Warning, TEXT("2"))
				m_gameMode->SetCurrentGameState(EGameState::PauseMenu);
			m_playerHud->RemoveFromParent();
			
			m_pauseHud = CreateWidget<UPauseHUD>(GetGameInstance(), m_pauseHudClass, FName("PauseWidget"));
			
			m_pauseHud->AddToViewport();
			GetWorld()->GetFirstPlayerController()->SetPause(true);
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}
		else if (m_gameMode->GetCurrentGameState() == EGameState::PauseMenu)
		{
			m_gameMode->SetCurrentGameState(EGameState::Playing);
			m_pauseHud->RemoveFromParent();

			m_playerHud = CreateWidget<UPlayerHUD>(GetGameInstance(), m_playerHudClass, FName("PlayerWidget"));
			
			m_playerHud->AddToViewport();
			GetWorld()->GetFirstPlayerController()->SetPause(false);
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
		}
	}
}

void APlayerControl::OpenMainMenu()
{
	m_mainMenuHud = CreateWidget<UMainmenuHUD>(GetGameInstance(), m_mainMenuHudClass, FName("MainMenuwidget"));

	m_mainMenuHud->AddToViewport();
	GetWorld()->GetFirstPlayerController()->SetPause(true);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

bool APlayerControl::IsDead()
{
	return m_dead;
}

bool APlayerControl::IsPlacingBomb()
{
	return m_isPlacingBomb;
}

void APlayerControl::SetPlacingBomb(bool _bool)
{
	m_isPlacingBomb = _bool;
}

bool APlayerControl::hasPlaceToSpawnBomb()
{
	FVector startTrace = GetActorLocation() - FVector(0, 20, 0);
	FVector endTrace = FVector(0, 40, 0) + startTrace;
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	CQP->AddIgnoredActor(this);
	TArray<FHitResult> hitResults;

	if (GetWorld()->LineTraceMultiByChannel(hitResults, startTrace, endTrace, ECC_Visibility, *CQP))
	{
		for (FHitResult& hitResult : hitResults)
		{
			AActor* actor = hitResult.GetActor();
			if (actor != nullptr)
			{
				return false;
			}
		}
	}

	startTrace = GetActorLocation() - FVector(20, 0, 0);
	endTrace = FVector(40, 0, 0) + startTrace;

	if (GetWorld()->LineTraceMultiByChannel(hitResults, startTrace, endTrace, ECC_Visibility, *CQP))
	{
		for (FHitResult& hitResult : hitResults)
		{
			AActor* actor = hitResult.GetActor();
			if (actor != nullptr)
			{
				return false;
			}
		}
	}
	return true;
}

void APlayerControl::OnInteract(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyHandler* enemy = Cast< AEnemyHandler>(OtherActor);
	if (enemy != nullptr)
		Damage();
}