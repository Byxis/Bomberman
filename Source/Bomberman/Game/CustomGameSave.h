#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"	
#include "Containers/Map.h"
#include "CustomGameSave.generated.h"

UCLASS()
class BOMBERMAN_API UCustomGameSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UCustomGameSave();

	void DefaultLeaderboard();
	void AddLeaderboard(int32 _score, FString _name);
	int32 GetScoreLeaderboard(int _i);
	int LenLeaderboard();
	void InsertLeaderboard(int32 _score, FString _name, int _i);
	void RemoveLeaderboard(int _i);
	TArray<int32> GetScores();
	TArray<FString> GetNames();

private:
	TArray<int32> m_scoreArray;
	TArray<FString> m_nameArray;
	
};
