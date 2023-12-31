#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"	
#include "Containers/Map.h"
#include "InputCoreTypes.h"
#include "CustomGameSave.generated.h"

UCLASS()
class BOMBERMAN_API UCustomGameSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UCustomGameSave();
	void Default();
	void DefaultLeaderboard();
	void AddLeaderboard(int32 _score, FString _name);
	int32 GetScoreLeaderboard(int _i);
	int LenLeaderboard();
	void InsertLeaderboard(int32 _score, FString _name, int _i);
	void RemoveLeaderboard(int _i);
	TArray<int32> GetScores();
	TArray<FString> GetNames();
	float GetMusicVolume();
	void SetMusicVolume(float _volume);
	float GetSFXVolume();
	void SetSFXVolume(float _volume);
	float GetJingleVolume();
	void SetJingleVolume(float _volume);

private:
	UPROPERTY()
	TArray<int32> m_scoreArray;
	UPROPERTY()
	TArray<FString> m_nameArray;

	UPROPERTY()
	float m_musicVolume = 1;
	UPROPERTY()
	float m_jingleVolume = 1;
	UPROPERTY()
	float m_sfxVolume = 1;
};
