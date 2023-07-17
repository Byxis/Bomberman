#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "CustomGameSave.h"
#include "CustomGameMode.generated.h"

UENUM()
enum class EGameState : uint8
{
	Menu,
	Playing,
	PauseMenu,
	End
};

class UBombHandler;

UCLASS()
class BOMBERMAN_API ACustomGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACustomGameMode();
	virtual ~ACustomGameMode() = default;

	void NextLevel();
	UFUNCTION(BlueprintPure)
	EGameState GetCurrentGameState() const;
	void SetCurrentGameState(EGameState _newState);
	int32 GetNbrOfWalls();
	void AddWall();
	void RemoveWall();
	bool HasExitSpawned();
	void SetExitSpawned(bool _bool);
	void AddBomb(UBombHandler* _bomb);
	void RemoveBomb(UBombHandler* _bomb);
	TArray<UBombHandler*> GetAllBombs();
	bool CanSpawnBonus();
	void AddSpawnedBonus(int _i);
	bool IsLevelFinished();
	void AddEnemy();
	void KillEnemy();
	int GetRemainingBonuses();
	

protected:
	virtual void BeginPlay() override;

private:
	EGameState m_currentGameState = EGameState::Playing;
	TArray<FString> m_levels;
	TArray<UBombHandler*> m_bombs;
	int32 m_nbrWalls = 0;
	int m_maxBonusRound = 0;
	int m_spawnedBonus = 0;
	int m_maxKilledEnemies = 0;
	int m_killedEnemies = 0;
	bool m_hasExitSpawned = false;

};
