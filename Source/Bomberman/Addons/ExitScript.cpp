#include "ExitScript.h"
#include <Bomberman/Game/CustomGameMode.h>
#include <Kismet/GameplayStatics.h>

AExitScript::AExitScript(const FObjectInitializer& _objectInitializer)
	:Super(_objectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	m_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));
	if (m_collision != nullptr)
	{
		m_collision->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		m_collision->SetHiddenInGame(false);
		m_collision->OnComponentBeginOverlap.AddDynamic(this, &AExitScript::OnPlayerEnterExit);

		RootComponent = m_collision;
	}
}

void AExitScript::BeginPlay()
{
	Super::BeginPlay();
}

void AExitScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExitScript::OnPlayerEnterExit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FString message = FString::Printf(TEXT("ENTERED"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, message);
	ACustomGameMode* GameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->SetCurrentGameState(EGameState::End);
	}
}

