#include "CustomGameMode.h"
#include "CustomGameState.h"
#include "./Bomberman/PlayerManager/PlayerControl.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include <Bomberman/Addons/DamageableActor.h>

ACustomGameMode::ACustomGameMode()
{
	GameStateClass = ACustomGameState::StaticClass();
	m_levels.Add("Level_1");
	m_levels.Add("Level_2");
	m_levels.Add("Level_3");
}

void ACustomGameMode::BeginPlay()
{
	Super::BeginPlay();
	SetCurrentGameState(EGameState::Playing);
	CountWalls();
}

void ACustomGameMode::NextLevel()
{
	GetGameState<ACustomGameState>()->SetLevel(GetGameState<ACustomGameState>()->GetLevel() + 1);
	FString current = GetWorld()->GetMapName();
	current.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	for (int i = 0; i < m_levels.Num(); i++)
	{
		if (current == m_levels[i])
		{
			if (i <= m_levels.Num() - 2)
			{
				UGameplayStatics::OpenLevel(GetWorld(), FName(m_levels[i + 1]));
				CountWalls();
				SetExitSpawned(false);
			}
		}
	}
}

int32 ACustomGameMode::GetNbrOfWalls()
{
	FString message = FString::Printf(TEXT("Walls: %f"), m_nbrWalls);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, message);
	return m_nbrWalls;
}

void ACustomGameMode::RemoveWall()
{
	m_nbrWalls -= 1;
}

void ACustomGameMode::SetCurrentGameState(EGameState _newState)
{
	m_currentGameState = _newState;
}

EGameState ACustomGameMode::GetCurrentGameState() const
{
	return m_currentGameState;
}
void ACustomGameMode::CountWalls()
{
	TArray<AActor*> walls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADamageableActor::StaticClass(), walls);
	int count = 0;
	for (AActor* wall : walls)
	{
		count += 1;
	}
	m_nbrWalls = count;
}

bool ACustomGameMode::HasExitSpawned()
{
	return m_hasExitSpawned;
}
void ACustomGameMode::SetExitSpawned(bool _bool)
{
	m_hasExitSpawned = _bool;
}

void ACustomGameMode::AddBomb(UBombHandler* _bomb)
{
	m_bombs.Add(_bomb);
}

void ACustomGameMode::RemoveBomb(UBombHandler* _bomb)
{
	m_bombs.Remove(_bomb);
}

TArray<UBombHandler*> ACustomGameMode::GetAllBombs()
{
	return m_bombs;
}
