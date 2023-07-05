#include "CustomGameMode.h"
#include "CustomGameState.h"
#include "./Bomberman/PlayerManager/PlayerControl.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"

ACustomGameMode::ACustomGameMode()
{
	GameStateClass = ACustomGameState::StaticClass();
}

void ACustomGameMode::BeginPlay()
{
	Super::BeginPlay();
	SetCurrentGameState(EGameState::Menu);
}

void ACustomGameMode::NextLevel()
{
	GetGameState<ACustomGameState>()->SetLevel(GetGameState<ACustomGameState>()->GetLevel() + 1);
}

void ACustomGameMode::SetCurrentGameState(EGameState _newState)
{
	m_currentGameState = _newState;
}

EGameState ACustomGameMode::GetCurrentGameState() const
{
	return m_currentGameState;
}