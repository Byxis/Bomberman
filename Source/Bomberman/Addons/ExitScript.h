#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "ExitScript.generated.h"

UCLASS()
class BOMBERMAN_API AExitScript : public AActor
{
	GENERATED_BODY()
	
public:
	AExitScript(const FObjectInitializer& objectInitializer);
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnPlayerEnterExit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UBoxComponent* m_collision = nullptr;
	ACustomGameMode* m_gameMode = nullptr;
	bool m_isOpened = false;
};
