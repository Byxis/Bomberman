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
		m_collision->OnComponentBeginOverlap.AddDynamic(this, &ABonus::OnPlayerTakeBonus);

		RootComponent = m_collision;
	}
}

void ABonus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_invicibilityTimer > 0)
	{
		m_invicibilityTimer -= DeltaTime;
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

void ABonus::Damage()
{
	if (m_invicibilityTimer <= 0)
	{
		Destroy();
	}
}

void ABonus::SetBonusType(int32 _type)
{
	switch (_type % 2)
	{
	default:
		m_bonusType = EBonus::Limit;
		break;
	case 1:
		m_bonusType = EBonus::Power;
		break;
	}
}
