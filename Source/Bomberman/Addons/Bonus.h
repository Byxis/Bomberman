#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "Bonus.generated.h"

UENUM()
enum class EBonus : uint8
{
	Power,
	Limit,
	Speed,
	Detonator,
	Vest,
	GhostWalls,
	MaxBonus
};

UCLASS()
class BOMBERMAN_API ABonus : public AActor
{
	GENERATED_BODY()
	
public:	
	ABonus(const FObjectInitializer& _objectInitializer);
	virtual void Tick(float _deltaTime) override;
	UFUNCTION()
	void OnPlayerTakeBonus(class UPrimitiveComponent* _hitComp, class AActor* _otherActor, class UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);
	void Damage();
	void SetBonusType(EBonus _type);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EBonus m_bonusType = EBonus::Limit;
	UBoxComponent* m_collision;
	float m_invicibilityTimer = 3.0f;
};
