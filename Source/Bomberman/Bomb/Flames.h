#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "Sound/SoundCue.h"	
#include "Components/AudioComponent.h"
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
	void PauseSFX(bool _bool);

protected:
	virtual void BeginPlay() override;

private:
	float m_timer = 2.5f;
	bool m_particuleActivated = true;
	UBoxComponent* m_collision = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USoundCue* m_flamesSFX = nullptr;
	UAudioComponent* m_flamesAudio = nullptr;
};
