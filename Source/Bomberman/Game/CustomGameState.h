#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CustomGameState.generated.h"

UCLASS()
class BOMBERMAN_API ACustomGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACustomGameState();
	virtual ~ACustomGameState() = default;

	void SetLevel(int32 _newLevel);
	int32 GetLevel();

private:
	int32 m_level = 1;
};
