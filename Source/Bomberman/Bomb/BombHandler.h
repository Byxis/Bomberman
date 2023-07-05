#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "./Bomberman/PlayerManager/PlayerControl.h"
#include "BombHandler.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class BOMBERMAN_API UBombHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBombHandler();
	void SetPower(int32 newPower);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	int32 m_power = 1.0f;
	float m_timer = 5.0f;
	bool m_collidable = true;
	AActor* m_owner = nullptr;
	UStaticMeshComponent* m_sphere = nullptr;
	bool m_isInitalisationDone = false;
	APlayerControl* m_player = nullptr;
	UMaterialInstanceDynamic* m_material = nullptr;

	void ApplyDestroyEffect();
	bool CheckAndDoDestruction(FVector StartTrace, FVector EndTrac);
};
