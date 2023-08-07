#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "CustomGameSave.h"
#include <Bomberman/Bomb/Flames.h>
#include "CustomGameMode.generated.h"

UENUM()
enum class EGameState : uint8
{
	Menu,
	StartingLevel,
	Playing,
	PauseMenu,
	LevelEnd,
	End,
	None
};

class UBombHandler;
class AEnemySpawner;
class UCustomGameInstance;

UCLASS()
class BOMBERMAN_API ACustomGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACustomGameMode();
	virtual ~ACustomGameMode() = default;

	virtual void Tick(float DeltaTime) override;

	void NextLevel();
	void OpenLevel(FName _name);
	void RestartLevel();
	UFUNCTION(BlueprintPure)
	EGameState GetCurrentGameState() const;
	void SetCurrentGameState(EGameState _newState);
	int32 GetNbrOfWalls();
	void AddWall(AActor* _actor);
	void SetActiveWallsGhost();
	void RemoveWall(AActor* _actor);
	bool HasExitSpawned();
	void SetExitSpawned(bool _bool);
	void AddBomb(UBombHandler* _bomb);
	void RemoveBomb(UBombHandler* _bomb);
	TArray<UBombHandler*> GetAllBombs();
	bool CanSpawnBonus();
	void AddSpawnedBonus(int _i);
	bool IsLevelFinished();
	void AddEnemy(AActor* _enemy);
	void RemoveEnemy(AActor* _enemy);
	TArray<AActor*> GetEnemies();
	void AddFlame(AFlames* _flame);
	void RemoveFlame(AFlames* _flame);
	int GetRemainingBonuses();
	int GetTimer();
	bool IsBonusLevel();
	void PauseAllSFX(bool _bool);
	void SetSFXSounds(float _amount);

protected:
	virtual void BeginPlay() override;

private:
	EGameState m_currentGameState = EGameState::None;
	TArray<FString> m_levels;
	TArray<UBombHandler*> m_bombs;
	TArray<AFlames*> m_flames;
	TArray<AActor*> m_walls;
	int m_maxBonusRound = 0;
	int m_spawnedBonus = 0;
	TArray<AActor*> m_enemies;
	bool m_hasExitSpawned = false;
	float m_timer = 10;//TODO: Repplace value by 200
	bool m_hasTimeExpired = false;
	UCustomGameInstance* m_gameInstance = nullptr;
	float m_endingLevelTimer = 3.0f;
	FName nextLevel = FName("");
	AEnemySpawner* m_spawner = nullptr;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_timerEnemy = nullptr;
};
