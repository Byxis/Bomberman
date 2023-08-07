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

void ABonus::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (m_invicibilityTimer > 0)
	{
		m_invicibilityTimer -= _deltaTime;
	}
}

void ABonus::OnPlayerTakeBonus(class UPrimitiveComponent* _hitComp, class AActor* _otherActor, class UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	APlayerControl* playerControl = Cast<APlayerControl>(_otherActor);

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
		case EBonus::Speed:
			playerControl->BonusSpeed();
			break;
		case EBonus::Detonator:
			playerControl->BonusDetonator();
			break;
		case EBonus::Vest:
			playerControl->BonusVest();
			break;
		case EBonus::GhostWalls:
			playerControl->BonusGhostWalls();
			break;
		default:
			break;
	}

	Destroy();
}

void ABonus::Damage()
{
	if (m_invicibilityTimer <= 0)
		Destroy();
}

void ABonus::SetBonusType(EBonus _type)
{
	m_bonusType = _type;
}
