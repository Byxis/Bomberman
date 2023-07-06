#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "./Bomberman/PlayerManager/PlayerControl.h"
#include <Components/BoxComponent.h>
#include "BombHandler.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class BOMBERMAN_API UBombHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBombHandler(const FObjectInitializer& _objectInitializer);
	void SetPower(int32 newPower);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnPlayerLeaveBomb(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

private:
	int32 m_power = 1;
	float m_timer = 5.0f;
	bool m_collidable = true;
	AActor* m_owner = nullptr;
	UStaticMeshComponent* m_sphere = nullptr;
	bool m_isInitalisationDone = false;
	APlayerControl* m_player = nullptr;
	UMaterialInstanceDynamic* m_material = nullptr;
	UBoxComponent* m_collision = nullptr;

	void ApplyDestroyEffect();
	bool CheckAndDoDestruction(FVector StartTrace, FVector EndTrac);
};
