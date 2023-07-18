#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameSave.h"
#include "Sound/SoundCue.h"
#include <Bomberman/Game/CustomGameMode.h>
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
	bool HasMaxedBombLimit();

	int32 GetBombPower();
	void AddBombPower(int32 _bombPower);
	bool HasMaxedBombPower();

	int32 GetScore();
	void SetScore(int32 _score);
	void AddScore(int32 _score);

	float GetSpeed();
	void AddSpeed(float _speed);
	bool HasMaxedSpeed();

	bool HasDetonatorBonus();
	void SetDetonatorBonus(bool _detonator);

	bool HasVestBonus();
	void SetVestBonus(bool _bool);

	bool HasGhostWallsBonus();
	void SetGhostWallsBonus(bool _bool);

	void ResetAll();
	void AddToLeaderboard(FString _name);
	void GetLeaderboard(TArray<int32>& _score, TArray<FString>& _name);
	void LoadLeaderboard();
	void SaveLeaderboard();
	void PlayMusic(EGameState _state);
	int GetLife();
	void AddLife(int _amount);

private:
	int32 m_bombLimit = 1;
	int32 m_maxBombLimit = 8;
	int32 m_bombPower = 1;
	int32 m_maxBombPower = 8;
	int32 m_score = 0;
	float m_speed = 0.9;
	float m_maxSpeed = 2.4;
	bool m_detonator = false;
	bool m_vest = false;
	bool m_ghostWalls = false;
	int m_life = 2;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USoundCue* m_menuMusic = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USoundCue* m_levelMusic = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USoundCue* m_secretLevelMusic = nullptr;

	UCustomGameSave* m_gameSave = nullptr;
};
