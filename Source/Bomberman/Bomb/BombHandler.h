#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "./Bomberman/PlayerManager/PlayerControl.h"
#include <Components/BoxComponent.h>	
#include "Components/AudioComponent.h"
#include "BombHandler.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class BOMBERMAN_API UBombHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBombHandler(const FObjectInitializer& _objectInitializer);
	void SetPower(int32 newPower);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Explode();

	UFUNCTION()
	void OnPlayerLeaveBomb(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	bool IsExploding();
	void PauseSFX(bool _bool);

protected:
	virtual void BeginPlay() override;

private:
	int32 m_power = 1;
	float m_timer = 3.0f;
	bool m_collidable = true;
	AActor* m_owner = nullptr;
	USkeletalMeshComponent* m_skeleton = nullptr;
	bool m_isInitalisationDone = false;
	APlayerControl* m_player = nullptr;
	UMaterialInstanceDynamic* m_material = nullptr;
	UBoxComponent* m_collisionExit = nullptr;
	UBoxComponent* m_collision = nullptr;
	bool m_exploding = false;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_flames = nullptr;

	void ApplyDestroyEffect();
	bool CanSpawnFlames(FVector _startTrace, FVector _endTrace);
	void CheckIfWall(FVector _startTrace, FVector _endTrace);

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USoundCue* m_smokeSFX = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USoundCue* m_explosionSFX = nullptr;

	UAudioComponent* m_smokeAudio = nullptr;
	UAudioComponent* m_explosionAudio = nullptr;
};
