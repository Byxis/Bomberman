#include "ExitScript.h"
#include <Bomberman/Game/CustomGameMode.h>
#include <Kismet/GameplayStatics.h>
#include <Bomberman/PlayerManager/PlayerControl.h>

AExitScript::AExitScript(const FObjectInitializer& _objectInitializer)
	:Super(_objectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	m_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));
	if (m_collision != nullptr)
	{
		m_collision->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		m_collision->SetHiddenInGame(false);
		m_collision->OnComponentBeginOverlap.AddDynamic(this, &AExitScript::OnPlayerEnterExit);

		RootComponent = m_collision;
	}
}

void AExitScript::OnPlayerEnterExit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACustomGameMode* gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
	APlayerControl* playerControl = Cast<APlayerControl>(OtherActor);
	if (gameMode != nullptr && gameMode->IsLevelFinished() && playerControl != nullptr)
	{
		gameMode->NextLevel();
	}
}

