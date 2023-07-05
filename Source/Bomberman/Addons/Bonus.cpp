#include "Bonus.h"
#include "./Bomberman/PlayerManager/PlayerControl.h"

ABonus::ABonus(const FObjectInitializer& _objectInitializer)
	:Super(_objectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	m_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));
	if (m_collision != nullptr)
	{
		m_collision->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		m_collision->SetHiddenInGame(false);
		m_collision->OnComponentBeginOverlap.AddDynamic(this, &ABonus::OnPlayerTakeBonus);

		RootComponent = m_collision;
	}
}

void ABonus::OnPlayerTakeBonus(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerControl* playerControl = Cast<APlayerControl>(OtherActor);

	if (playerControl == nullptr)
		return;

	switch (m_bonusType)
	{
	case EBonus::Power:
		playerControl->BonusBombPower();
		break;
	case EBonus::Limit:
		playerControl->BonusBombLimit();
		break;
	default:
		break;
	}
	Destroy();
}