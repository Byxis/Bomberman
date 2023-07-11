#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"


UCLASS()
class BOMBERMAN_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UCustomGameInstance();

public:
	virtual void Init() override;

	int32 GetBombLimit();
	void AddBombLimit(int32 _bombLimit);
	int32 GetBombPower();
	void AddBombPower(int32 _bombPower);
	int32 GetScore();
	void SetScore(int32 _score);
	void AddScore(int32 _score);

private:
	int32 m_bombLimit = 1;
	int32 m_bombPower = 1;
	int32 m_score = 0;

};
