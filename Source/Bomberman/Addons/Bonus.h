#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "Bonus.generated.h"

UENUM()
enum class EBonus : uint8
{
	Power,
	Limit
};

UCLASS()
class BOMBERMAN_API ABonus : public AActor
{
	GENERATED_BODY()
	
public:	
	ABonus(const FObjectInitializer& objectInitializer);
	UFUNCTION()
	void OnPlayerTakeBonus(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EBonus m_bonusType = EBonus::Limit;
	UBoxComponent* m_collision;
};