#include "CustomGameMode.h"
#include "CustomGameState.h"
#include "./Bomberman/PlayerManager/PlayerControl.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "./Bomberman/Bomb/BombHandler.h"
#include <Bomberman/Addons/DamageableActor.h>
#include "./Bomberman/Enemy/EnemySpawner.h"


ACustomGameMode::ACustomGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	GameStateClass = ACustomGameState::StaticClass();
	int32 random = 1;
	m_maxBonusRound = random;
	m_spawnedBonus = 0;

	m_levels.Add("Level_1");
	m_levels.Add("Level_2");
	m_levels.Add("Level_3");
	m_levels.Add("Level_4");
	m_levels.Add("Level_5");
	m_levels.Add("Level_Bonus_1");
	m_levels.Add("Level_6");
}

void ACustomGameMode::BeginPlay()
{
	m_spawner = Cast<AEnemySpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawner::StaticClass()));
	m_gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true).Equals(TEXT("Level_0")))
	{
		SetCurrentGameState(EGameState::Menu);
	}
	else if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true).Equals(TEXT("Level_End")))
	{
		SetCurrentGameState(EGameState::End);
	}
	else
	{
		if (IsBonusLevel())
		{
			if(m_spawner != nullptr)
				m_spawner->SpawnEnemies(true);
			m_timer = 30.0f;
		}
		SetCurrentGameState(EGameState::Playing);
	}
	Super::BeginPlay();
}

void ACustomGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (nextLevel != FName(""))
	{
		if (m_endingLevelTimer <= 0)
		{
			OpenLevel(nextLevel);
			return;
		}
		m_endingLevelTimer -= DeltaTime;
	}

	if (m_timer > 0 && m_currentGameState == EGameState::Playing)
		m_timer -= DeltaTime;
	
	if (!m_hasTimeExpired && m_timer < 0 && m_currentGameState == EGameState::Playing)
	{
		if (IsBonusLevel())
		{
			NextLevel();
			return;
		}
		for (AActor* enemy : m_enemies)
		{
			enemy->Destroy();
		}
		m_enemies.Empty();

		if(m_spawner != nullptr)
			m_spawner->SpawnEnemies(false);

		m_hasTimeExpired = true;
		SetActiveWallsGhost();
	}
}

void ACustomGameMode::NextLevel()
{
	if (!IsBonusLevel())
	{
		GetGameState<ACustomGameState>()->SetLevel(GetGameState<ACustomGameState>()->GetLevel() + 1);
		if (m_gameInstance != nullptr)
			m_gameInstance->AddLife(1);
	}
	FString current = GetWorld()->GetMapName();
	current.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	for (int i = 0; i < m_levels.Num(); i++)
	{
		if (current == m_levels[i])
		{
			if (i <= m_levels.Num() - 2)
			{
				nextLevel = FName(m_levels[i + 1]);
				SetCurrentGameState(EGameState::LevelEnd);
				SetExitSpawned(false);
			}
			if (i == m_levels.Num() - 1)
			{
				nextLevel = FName("Level_End");
				SetCurrentGameState(EGameState::LevelEnd);
				SetExitSpawned(false);
			}
			break;
		}
	}
}

void ACustomGameMode::OpenLevel(FName _name)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(_name));
}

void ACustomGameMode::RestartLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(UGameplayStatics::GetCurrentLevelName(GetWorld(), true)));
}

int32 ACustomGameMode::GetNbrOfWalls()
{
	return m_walls.Num();
}

void ACustomGameMode::AddWall(AActor* _actor)
{
	m_walls.Add(_actor);
	UStaticMeshComponent* mesh = _actor->GetComponentByClass<UStaticMeshComponent>();
	if (mesh != nullptr)
	{
		if (m_hasTimeExpired && m_gameInstance != nullptr)
		{
			if (m_gameInstance->HasGhostWallsBonus())
			{
				mesh->SetCollisionProfileName(FName("GhostWallsAll"));
			}
			else
			{
				mesh->SetCollisionProfileName(FName("GhostWallsEnemy"));
			}
		}
		else
		{
			if (m_gameInstance->HasGhostWallsBonus())
			{
				mesh->SetCollisionProfileName(FName("GhostWallsPlayer"));
			}
		}
	}
	
}

void ACustomGameMode::SetActiveWallsGhost()
{
	for (AActor* actor : m_walls)
	{
		UStaticMeshComponent* mesh = actor->GetComponentByClass<UStaticMeshComponent>();
		if (mesh != nullptr)
		{
			if (m_hasTimeExpired)
			{
				if (m_gameInstance->HasGhostWallsBonus())
				{
					mesh->SetCollisionProfileName(FName("GhostWallsAll"));
				}
				else
				{
					mesh->SetCollisionProfileName(FName("GhostWallsEnemy"));
				}
			}
			else
			{
				if (m_gameInstance->HasGhostWallsBonus())
				{
					mesh->SetCollisionProfileName(FName("GhostWallsPlayer"));
				}
			}
		}
	}
}

void ACustomGameMode::RemoveWall(AActor* _actor)
{
	m_walls.Remove(_actor);
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
	return m_enemies.Num() == 0;
}

void ACustomGameMode::AddEnemy(AActor* _enemy)
{
	m_enemies.Add(_enemy);
}

void ACustomGameMode::RemoveEnemy(AActor* _enemy)
{
	m_enemies.Remove(_enemy);
}

TArray<AActor*> ACustomGameMode::GetEnemies()
{
	return m_enemies;
}

void ACustomGameMode::AddFlame(AFlames* _flame)
{
	m_flames.Add(_flame);
}

void ACustomGameMode::RemoveFlame(AFlames* _flame)
{
	m_flames.Remove(_flame);
}

int ACustomGameMode::GetRemainingBonuses()
{
	return m_maxBonusRound- m_spawnedBonus;
}

int ACustomGameMode::GetTimer()
{
	return (int) m_timer;
}

bool ACustomGameMode::IsBonusLevel()
{
	return UGameplayStatics::GetCurrentLevelName(GetWorld(), true).Contains(TEXT("Level_Bonus"));
}

void ACustomGameMode::PauseAllSFX(bool _bool)
{
	for (UBombHandler* bomb : m_bombs)
	{
		bomb->PauseSFX(_bool);
	}

	for (AFlames* flame : m_flames)
	{
		flame->PauseSFX(_bool);
	}
}

void ACustomGameMode::SetSFXSounds(float _amount)
{
	for (UBombHandler* bomb : m_bombs)
	{
		bomb->SetSFXVolume(_amount);
	}

	for (AFlames* flame : m_flames)
	{
		flame->SetSFXVolume(_amount);
	}
}
