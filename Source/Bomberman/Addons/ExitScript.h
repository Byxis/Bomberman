#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "./Bomberman/Game/CustomGameMode.h"
#include "ExitScript.generated.h"

UCLASS()
class BOMBERMAN_API AExitScript : public AActor
{
	GENERATED_BODY()
	
public:
	AExitScript(const FObjectInitializer& _objectInitializer);
	virtual void Tick(float _deltaTime) override;
	UFUNCTION()
	void OnPlayerEnterExit(class UPrimitiveComponent* _hitComp, class AActor* _otherActor, class UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult& _sweepResult);
	bool IsOpened();

private:
	UBoxComponent* m_collision = nullptr;
	ACustomGameMode* m_gameMode = nullptr;
	bool m_isOpened = false;
};
