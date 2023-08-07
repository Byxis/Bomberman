#include "ExitScript.h"
#include <Bomberman/Game/CustomGameMode.h>
#include <Kismet/GameplayStatics.h>
#include <Bomberman/PlayerManager/PlayerControl.h>

AExitScript::AExitScript(const FObjectInitializer& _objectInitializer)
	:Super(_objectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	m_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));
	if (m_collision != nullptr)
	{
		m_collision->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		m_collision->OnComponentBeginOverlap.AddDynamic(this, &AExitScript::OnPlayerEnterExit);

		RootComponent = m_collision;
	}
}

void AExitScript::Tick(float _deltaTime)
{
	if (m_gameMode == nullptr)
	{
		m_gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
	}

	if (m_gameMode->IsLevelFinished() && !m_isOpened)
	{
		m_isOpened = true;
	}
	else if (!m_gameMode->IsLevelFinished() && m_isOpened)
	{
		m_isOpened = false;
	}
}

void AExitScript::OnPlayerEnterExit(class UPrimitiveComponent* _hitComp, class AActor* _otherActor, class UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult& _sweepResult)
{
	APlayerControl* playerControl = Cast<APlayerControl>(_otherActor);
	if (playerControl != nullptr)
		return;

	if (m_gameMode == nullptr)
	{
		m_gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
	}
	if (m_gameMode != nullptr && m_gameMode->IsLevelFinished())
	{
		m_gameMode->NextLevel();
	}
}

bool AExitScript::IsOpened()
{
	return m_isOpened;
}

