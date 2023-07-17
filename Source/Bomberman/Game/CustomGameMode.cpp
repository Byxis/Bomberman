#include "CustomGameMode.h"
#include "CustomGameState.h"
#include "./Bomberman/PlayerManager/PlayerControl.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include <Bomberman/Addons/DamageableActor.h>

ACustomGameMode::ACustomGameMode()
{
	GameStateClass = ACustomGameState::StaticClass();
	int32 random = 1+std::rand() % 2;
	m_maxBonusRound = random;
	m_spawnedBonus = 0;

	m_levels.Add("Level_1");
	m_levels.Add("Level_2");
	m_levels.Add("Level_3");
}

void ACustomGameMode::BeginPlay()
{
	if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true).Equals("Level_0"))
	{
		SetCurrentGameState(EGameState::Menu);
	}
	else
	{
		SetCurrentGameState(EGameState::Playing);
	}

	UCustomGameSave* m_gameSave = Cast<UCustomGameSave>(UGameplayStatics::CreateSaveGameObject(UCustomGameSave::StaticClass()));
	UCustomGameInstance* gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(UCustomGameInstance::StaticClass()));
	if (gameInstance != nullptr)
		gameInstance->PlayMusic(m_currentGameState);
	Super::BeginPlay();
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
				SetExitSpawned(false);
			}
		}
	}
}

int32 ACustomGameMode::GetNbrOfWalls()
{
	return m_nbrWalls;
}

void ACustomGameMode::AddWall()
{
	m_nbrWalls += 1;
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

bool ACustomGameMode::CanSpawnBonus()
{
	return m_spawnedBonus < m_maxBonusRound;
}

void ACustomGameMode::AddSpawnedBonus(int _i)
{
	m_spawnedBonus += _i;
}

bool ACustomGameMode::IsLevelFinished()
{
	return m_killedEnemies >= m_maxKilledEnemies;
}

void ACustomGameMode::AddEnemy()
{
	m_maxKilledEnemies += 1;
}

void ACustomGameMode::KillEnemy()
{
	m_killedEnemies += 1;
}

int ACustomGameMode::GetRemainingBonuses()
{
	return m_maxBonusRound- m_spawnedBonus;
}
