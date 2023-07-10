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

APlayerControl::APlayerControl()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(45, 45);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);

	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	SpringArmComp->bUsePawnControlRotation = false;
}

void APlayerControl::BeginPlay()
{
	Super::BeginPlay();
	m_playerHud = CreateWidget<UPlayerHUD>(GetGameInstance(), m_playerHudClass, FName("PlayerWidget"));
	m_playerHud->AddToViewport();

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

	SpringArmComp->SetWorldLocationAndRotation(FVector(1700.0f, 900.0f, 1500.0f), FRotator(-90.0f, 0.0f, -90.0f));
}

void APlayerControl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerControl::MoveForward);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerControl::MoveRight);
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &APlayerControl::SpawnBomb);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerControl::PauseGame);
}

void APlayerControl::MoveForward(float _axis)
{
	if (!m_dead && m_canMove)
	{
		if (_axis > 0.2)
			_axis = 1;
		if (_axis < -0.2)
			_axis = -1;
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, _axis *0.8f);
	}
}

void APlayerControl::MoveRight(float _axis)
{
	if (!m_dead && m_canMove)
	{
		if (_axis > 0.2)
			_axis = 1;
		if (_axis < -0.2)
			_axis = -1;
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, _axis *-0.8f);
	}
}

void APlayerControl::SpawnBomb()
{
	if (m_maxPlacedBomb > 0 && m_canPlaceBomb)
	{
		const FVector location(FGenericPlatformMath::RoundToInt((GetActorLocation().X - 50) / 100) * 100 + 50,
			FGenericPlatformMath::RoundToInt((GetActorLocation().Y - 50) / 100) * 100 + 50,
			50);
		const FRotator rotation = GetActorRotation();

		FString message = FString::Printf(TEXT("Location: X=%f, Y=%f, Z=%f, remaining:%f"), location.X, location.Y, location.Z, m_maxPlacedBomb);
		AActor* spawnedbomb = GetWorld()->SpawnActor<AActor>(m_bomb, location, rotation);
		if (spawnedbomb != nullptr)
		{
			SetCanPlaceBomb(false);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, message);
			UBombHandler* bombHandler = spawnedbomb->GetComponentByClass<UBombHandler>();
			if (bombHandler != nullptr)
				bombHandler->SetPower(m_power);

			m_maxPlacedBomb--;
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
	Destroy();
}


void APlayerControl::BonusBombPower()
{
	m_power++;
}

void APlayerControl::BonusBombLimit()
{
	m_maxPlacedBomb++;
}

void APlayerControl::ActualiseScore(int32 _score)
{
	if (m_playerHud != nullptr)
	{
		m_playerHud->SetScore(_score);
	}
	if (m_pauseHud != nullptr)
	{
		m_pauseHud->SetScore(_score);
	}
}

void APlayerControl::PauseGame()
{
	ACustomGameMode* gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
	if (gameMode != nullptr)
	{
		if (gameMode->GetCurrentGameState() == EGameState::Playing)
		{
			gameMode->SetCurrentGameState(EGameState::PauseMenu);
			m_playerHud->RemoveFromParent();
			m_pauseHud = CreateWidget<UPauseHUD>(GetGameInstance(), m_pauseHudClass, FName("PauseWidget"));
			m_pauseHud->AddToViewport();
			GetWorld()->GetFirstPlayerController()->SetPause(true);
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}
		else if (gameMode->GetCurrentGameState() == EGameState::PauseMenu)
		{
			gameMode->SetCurrentGameState(EGameState::Playing);
			m_pauseHud->RemoveFromParent();
			m_playerHud = CreateWidget<UPlayerHUD>(GetGameInstance(), m_playerHudClass, FName("PlayerWidget"));
			m_playerHud->AddToViewport();
			GetWorld()->GetFirstPlayerController()->SetPause(false);
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
		}
	}
}
