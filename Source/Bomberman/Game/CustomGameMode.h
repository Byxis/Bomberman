#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "CustomGameMode.generated.h"

UENUM()
enum class EGameState : uint8
{
	Menu,
	Playing,
	PauseMenu,
	End
};

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
	void RemoveWall();
	bool HasExitSpawned();
	void SetExitSpawned(bool _bool);
	void AddScore(int32 _score);
	int32 GetScore();

protected:
	virtual void BeginPlay() override;

private:
	EGameState m_currentGameState = EGameState::Menu;
	TArray<FString> levels;
	int32 m_nbrWalls = 0;
	bool m_hasExitSpawned = false;

	void CountWalls();
};
