#include "CustomGameMode.h"
#include "CustomGameState.h"
#include "./Bomberman/PlayerManager/PlayerControl.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include <Bomberman/Addons/DamageableActor.h>

ACustomGameMode::ACustomGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

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
		if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true).Equals("Level_Bonus"))
		{
			m_timer = 30.0f;
		}
		SetCurrentGameState(EGameState::Playing);
	}
	Super::BeginPlay();

	m_gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (m_gameInstance != nullptr)
	{
		m_gameInstance->PlayMusic(m_currentGameState);
	}
}

void ACustomGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_timer > 0 && m_currentGameState == EGameState::Playing)
		m_timer -= DeltaTime;
	
	if (!m_hasTimeExpired && m_timer < 0 && m_currentGameState == EGameState::Playing)
		m_hasTimeExpired = true;

	if (m_hasTimeExpired)
	{
		if (IsBonusLevel())
			NextLevel();
		else
		{
			//TODO: spawn hard enemies
		}
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
				UGameplayStatics::OpenLevel(GetWorld(), FName(m_levels[i + 1]));
				SetExitSpawned(false);
			}
		}
	}
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
	if (m_gameInstance->HasGhostWallsBonus())
	{
		UStaticMeshComponent* mesh = _actor->GetComponentByClass<UStaticMeshComponent>();
		if (mesh != nullptr)
			mesh->SetCollisionProfileName(FName("GhostWalls"));
	}
}

void ACustomGameMode::SetActiveWallsGhost()
{
	if (m_gameInstance->HasGhostWallsBonus())
	{
		for (AActor* actor : m_walls)
		{
			UStaticMeshComponent* mesh = actor->GetComponentByClass<UStaticMeshComponent>();
			if (mesh != nullptr)
				mesh->SetCollisionProfileName(FName("GhostWalls"));
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

int ACustomGameMode::GetTimer()
{
	return (int) m_timer;
}

bool ACustomGameMode::IsBonusLevel()
{
	return UGameplayStatics::GetCurrentLevelName(GetWorld(), true).Equals(TEXT("Level_Bonus"));
}
