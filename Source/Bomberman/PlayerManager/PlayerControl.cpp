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
	SpringArmComp->SetWorldLocationAndRotation(FVector(1650, 850, 1650.0f), FRotator(-90.0f, 0.0f, -90.0f));

	LoadPlayerControl();
	/*
	if (m_touchInterface != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		GetWorld()->GetFirstPlayerController()->SetVirtualJoystickVisibility(true);
		GetWorld()->GetFirstPlayerController()->ActivateTouchInterface(m_touchInterface);
	}*/
}

void APlayerControl::LoadPlayerControl()
{
	m_gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	AGameModeBase* gm = UGameplayStatics::GetGameMode(GetWorld());
	m_gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	m_maxPlacedBomb = m_gameInstance->GetBombLimit();

	if (m_gameMode == nullptr)
	{
		return;
	}
	if (m_gameMode->GetCurrentGameState() == EGameState::None)
	{
		return;
	}

	if (m_gameMode->GetCurrentGameState() == EGameState::Menu)
	{
		SpringArmComp->SetWorldLocationAndRotation(FVector(1650, 850, 1650.0f), FRotator(0, 0, 0));
		OpenMainMenu();
		m_isLoaded = true;
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
		m_isLoaded = true;
	}
	else if (m_gameMode->GetCurrentGameState() == EGameState::StartingLevel)
	{
		m_startingHud = CreateWidget<UStartingHUD>(GetGameInstance(), m_startingHudClass, FName("StargingLevelWidget"));
		PlayJingle();
		if (m_startingHud != nullptr)
		{
			m_startingHud->AddToViewport();
		}
		m_isLoaded = true;
	}
	else if (m_gameMode->GetCurrentGameState() == EGameState::End)
	{
		m_startDelay = 15;
		m_isLoaded = true;
	}
	PlayMusic();
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
	if (!m_isLoaded)
	{
		LoadPlayerControl();
		return;
	}
	SpringArmComp->SetWorldLocationAndRotation(FVector(1650, 850, 1650.0f), FRotator(-90.0f, 0.0f, -90.0f));

	if (m_startDelay > 0 && m_gameMode != nullptr && (m_gameMode->GetCurrentGameState() == EGameState::StartingLevel || m_gameMode->GetCurrentGameState() == EGameState::End) )
	{
		m_startDelay -= DeltaTime;
		return;
	}
	else if (m_startDelay < 0 && m_gameMode != nullptr && m_gameMode->GetCurrentGameState() == EGameState::StartingLevel)
	{
		m_gameMode->SetCurrentGameState(EGameState::Playing);
		m_startingHud->RemoveFromParent();

		m_playerHud = CreateWidget<UPlayerHUD>(GetGameInstance(), m_playerHudClass, FName("PlayerWidget"));
		if (m_playerHud != nullptr)
		{
			m_playerHud->AddToViewport();
		}
		ActualiseLife();
		ActualiseScore();

		PlayMusic();
	}
	else if (m_startDelay < 0 && m_gameMode != nullptr && m_gameMode->GetCurrentGameState() == EGameState::End)
	{
		m_endHud = CreateWidget<UGameoverHUD>(GetGameInstance(), m_endHudClass, FName("PlayerWidget"));
		if (m_endHud != nullptr)
		{
			m_endHud->AddToViewport();
			GetWorld()->GetFirstPlayerController()->SetPause(true);
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}
	}
	if (m_gameMode != nullptr && m_gameMode->GetCurrentGameState() == EGameState::LevelEnd && !m_hasEnded)
	{
		m_hasEnded = true;
		PauseMusic(true);
		PlayMusic();
	}

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
	if (!m_dead && m_gameMode->GetCurrentGameState() == EGameState::Playing)
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
	if (!m_dead && m_gameMode->GetCurrentGameState() == EGameState::Playing)
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
	if (m_maxPlacedBomb > 0 && m_canPlaceBomb && !m_dead && hasPlaceToSpawnBomb() && m_gameMode->GetCurrentGameState() == EGameState::Playing)
	{
		const FVector location(FGenericPlatformMath::RoundToInt((GetActorLocation().X - 50) / 100) * 100 + 50,
			FGenericPlatformMath::RoundToInt((GetActorLocation().Y - 50) / 100) * 100 + 50,
			50);
		const FRotator rotation = GetActorRotation();
		AActor* spawnedbomb = GetWorld()->SpawnActor<AActor>(m_bomb, location, rotation);
		if (spawnedbomb != nullptr)
		{
			SetCanPlaceBomb(false);
			m_maxPlacedBomb--;
			UBombHandler* bombHandler = spawnedbomb->GetComponentByClass<UBombHandler>();
			if (bombHandler != nullptr)
				bombHandler->SetPower(m_gameInstance->GetBombPower());

			SetPlacingBomb(true);			
		}
	}
}
void APlayerControl::Detonatebomb()
{
	if (m_gameInstance->HasDetonatorBonus() && !m_dead && m_gameMode->GetCurrentGameState() == EGameState::Playing)
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
	if (!m_dead && m_gameMode != nullptr && !m_gameMode->IsBonusLevel() && m_gameMode->GetCurrentGameState() == EGameState::Playing)
	{
		PauseMusic(true);
		m_dead = true;
		PlayJingle();
		if (m_gameInstance != nullptr && m_gameInstance->GetLife() > 0)
		{
			m_gameInstance->AddLife(-1);
			m_gameInstance->RemoveRandomBonus();
			ActualiseLife();
			m_isRespawning = true;
			m_respawningTimer = 8.0f;
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
			m_gameMode->SetCurrentGameState(EGameState::PauseMenu);
			m_playerHud->RemoveFromParent();
			PauseMusic(true);
			PauseJingle(true);
			m_gameMode->PauseAllSFX(true);
			
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
			m_gameMode->PauseAllSFX(false);
			PauseJingle(false);
			if(!m_dead)
				PauseMusic(false);
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

void APlayerControl::OpenOptionsMenu()
{
	if (m_gameMode->GetCurrentGameState() == EGameState::PauseMenu)
	{
		m_pauseHud->RemoveFromParent();

		m_optionHud = CreateWidget<UOptionHUD>(GetGameInstance(), m_optionHudClass, FName("OptionWidget"));

		m_optionHud->AddToViewport();
	}
	else if (m_gameMode->GetCurrentGameState() == EGameState::Menu)
	{
		m_mainMenuHud->RemoveFromParent();

		m_optionHud = CreateWidget<UOptionHUD>(GetGameInstance(), m_optionHudClass, FName("OptionWidget"));

		m_optionHud->AddToViewport();
	}
}

void APlayerControl::CloseOptionsMenu()
{
	if (m_gameMode->GetCurrentGameState() == EGameState::PauseMenu)
	{
		m_optionHud->RemoveFromParent();

		m_pauseHud = CreateWidget<UPauseHUD>(GetGameInstance(), m_pauseHudClass, FName("PauseWidget"));

		m_pauseHud->AddToViewport();
	}
	else if (m_gameMode->GetCurrentGameState() == EGameState::Menu)
	{
		m_optionHud->RemoveFromParent();

		m_mainMenuHud = CreateWidget<UMainmenuHUD>(GetGameInstance(), m_mainMenuHudClass, FName("MainMenuwidget"));

		m_mainMenuHud->AddToViewport();
	}
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

void APlayerControl::PlayMusic()
{
	if (m_gameMode == nullptr)
		return;

	switch (m_gameMode->GetCurrentGameState())
	{
		default:
		{
			break;
		}
		case EGameState::Menu:
		{
			if (m_menuMusic != nullptr)
			{
				m_currentMusic = UGameplayStatics::SpawnSound2D(GetWorld(), m_menuMusic, m_gameInstance->GetMusicVolume(), 1, 0);
			}
			break;
		}
		case EGameState::Playing:
		{
			if (m_gameMode->IsBonusLevel())
			{
				if (m_bonusLevelMusic != nullptr)
				{
					m_currentMusic = UGameplayStatics::SpawnSound2D(GetWorld(), m_bonusLevelMusic, m_gameInstance->GetMusicVolume(), 1, 0);
				}
			}
			else
			{
				if (m_levelMusic != nullptr)
				{
					m_currentMusic = UGameplayStatics::SpawnSound2D(GetWorld(), m_levelMusic, m_gameInstance->GetMusicVolume(), 1, 0);
				}
			}
			break;
		}
	}
}

void APlayerControl::PauseMusic(bool _bool)
{
	if (m_currentMusic != nullptr)
	{
		m_currentMusic->SetPaused(_bool);
	}
	if (m_currentJingle != nullptr)
	{
		m_currentJingle->SetPaused(_bool);
	}
}

void APlayerControl::SetMusicVolume(float _volume)
{
	if (m_currentMusic != nullptr)
	{
		m_currentMusic->SetVolumeMultiplier(_volume);
	}
}

void APlayerControl::PlayJingle()
{
	if (m_gameMode != nullptr && m_gameMode->GetCurrentGameState() == EGameState::StartingLevel)
	{
		if (m_startingJingle != nullptr)
		{
			m_currentJingle = UGameplayStatics::SpawnSound2D(GetWorld(), m_startingJingle, m_gameInstance->GetJingleVolume(), 1, 0);
		}
	}
	else if (m_dead)
	{
		if (m_deathJingle != nullptr)
		{
			m_currentJingle = UGameplayStatics::SpawnSound2D(GetWorld(), m_deathJingle, m_gameInstance->GetJingleVolume(), 1, 0);
		}
	}
	else if (m_hasEnded)
	{
		if (m_deathJingle != nullptr)
		{
			m_currentJingle = UGameplayStatics::SpawnSound2D(GetWorld(), m_deathJingle, m_gameInstance->GetJingleVolume(), 1, 0);
		}
	}
}

void APlayerControl::PauseJingle(bool _bool)
{
	if (m_currentJingle != nullptr)
	{
		m_currentJingle->SetPaused(_bool);
	}
}

void APlayerControl::SetJingleVolume(float _volume)
{
	if (m_currentJingle != nullptr)
	{
		m_currentJingle->SetVolumeMultiplier(_volume);
	}
}
