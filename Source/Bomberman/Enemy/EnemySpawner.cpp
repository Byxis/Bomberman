#include "EnemySpawner.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_isBonusLevel && m_gameMode != nullptr && m_enemy != nullptr)
	{
		if (m_gameMode->GetEnemies().Num() < 10)
		{
			const FRotator rotation(0, 0, 0);
			while (m_gameMode->GetEnemies().Num() < 10)
			{
				int32 random = std::rand() % m_spawnLocations.Num();
				GetWorld()->SpawnActor<AActor>(m_enemy, m_spawnLocations[random], rotation);
			}
		}
	}
}

void AEnemySpawner::SpawnEnemies(bool isBonusLevel)
{
	if (m_enemy != nullptr)
	{
		const FRotator rotation(0, 0, 0);
		for (FVector location : m_spawnLocations)
		{
			GetWorld()->SpawnActor<AActor>(m_enemy, location, rotation);
		}
	}
	m_isBonusLevel = isBonusLevel;
	if (isBonusLevel)
	{
		m_gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
	}
}
