#include "CustomGameState.h"
#include "CustomGameMode.h"

ACustomGameState::ACustomGameState()
{
	m_level = 0;
}

void ACustomGameState::SetLevel(int32 _newLevel)
{
	m_level = _newLevel;
}

int32 ACustomGameState::GetLevel()
{
	return m_level;
}