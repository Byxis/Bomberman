#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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

protected:
	virtual void BeginPlay() override;

private:
	EGameState m_currentGameState;
};
