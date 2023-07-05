#include "PlayerControl.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "kismet/GameplayStatics.h"
#include "Engine/GameEngine.h"
#include "GameFramework/SpringArmComponent.h"
#include "./Bomberman/Bomb/BombHandler.h"

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
}

void APlayerControl::MoveForward(float _axis)
{
	if (!m_dead && m_canMove)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, _axis);
	}
}

void APlayerControl::MoveRight(float _axis)
{
	if (!m_dead && m_canMove)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, _axis *-1);
	}
}

void APlayerControl::SpawnBomb()
{
	if (m_maxPlacedBomb > 0)
	{
		const FVector location(FGenericPlatformMath::RoundToInt((GetActorLocation().X - 50) / 100) * 100 + 50,
			FGenericPlatformMath::RoundToInt((GetActorLocation().Y - 50) / 100) * 100 + 50,
			FGenericPlatformMath::RoundToInt((GetActorLocation().Z) / 100) * 100 + 50);
		const FRotator rotation = GetActorRotation();
		FString message = FString::Printf(TEXT("Location: X=%f, Y=%f, Z=%f, remaining:%f"), location.X, location.Y, location.Z, m_maxPlacedBomb);
		AActor* spawnedbomb = GetWorld()->SpawnActor<AActor>(m_bomb, location, rotation);

		m_maxPlacedBomb--;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, message);
		spawnedbomb->GetComponentByClass<UBombHandler>()->SetPower(m_power);
	}
}

void APlayerControl::GiveBackBomb()
{
	m_maxPlacedBomb++;
}

void APlayerControl::Damage()
{
	UE_LOG(LogTemp, Warning, TEXT("AIE"))
	Destroy();
}