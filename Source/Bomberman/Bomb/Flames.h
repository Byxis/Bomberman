#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "Flames.generated.h"

UCLASS()
class BOMBERMAN_API AFlames : public AActor
{
	GENERATED_BODY()
	
public:	
	AFlames(const FObjectInitializer& _objectInitializer);
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnEntityInteract(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	float m_timer = 2.5f;
	bool m_particuleActivated = true;
	UBoxComponent* m_collision = nullptr;
};
