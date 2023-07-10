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
	UFUNCTION()
	void OnPlayerEnterExit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UBoxComponent* m_collision = nullptr;
};
