#include "DamageableActor.h"
#include "GameFramework/Character.h"
#include <Bomberman/Game/CustomGameMode.h>
#include <Bomberman/Game/CustomGameInstance.h>
#include "Bonus.h"

ADamageableActor::ADamageableActor()
{}

void ADamageableActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);

	m_health = m_sartingHealth;
	m_mesh = GetComponentByClass<UStaticMeshComponent>();

	if (m_health == 1)
	{
		if (m_mesh != nullptr && m_destroyedBricks != nullptr)
		{
			m_mesh->SetStaticMesh(m_destroyedBricks);
		}
	}
	ACustomGameMode* gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
	if (gameMode != nullptr)
	{
		gameMode->AddWall(this);
	}
}

std::list<EBonus> ADamageableActor::GetAvailablebonusWithWeight(UCustomGameInstance* _gameInstance)
{
	std::list<EBonus> bonusList;
	if (!_gameInstance->HasMaxedBombLimit() && m_bonusLimit != nullptr)
	{
		bonusList.push_front(EBonus::Limit);
		bonusList.push_front(EBonus::Limit);
		bonusList.push_front(EBonus::Limit);
		bonusList.push_front(EBonus::Limit);
		bonusList.push_front(EBonus::Limit);
		bonusList.push_front(EBonus::Limit);
	}
	if (!_gameInstance->HasMaxedBombPower() && m_bonusPower != nullptr)
	{
		bonusList.push_front(EBonus::Power);
		bonusList.push_front(EBonus::Power);
		bonusList.push_front(EBonus::Power);
		bonusList.push_front(EBonus::Power);
		bonusList.push_front(EBonus::Power);
		bonusList.push_front(EBonus::Power);
	}
	if (!_gameInstance->HasMaxedSpeed() && m_bonusSpeed != nullptr)
	{
		bonusList.push_front(EBonus::Speed);
		bonusList.push_front(EBonus::Speed);
		bonusList.push_front(EBonus::Speed);
		bonusList.push_front(EBonus::Speed);
	}
	if (!_gameInstance->HasDetonatorBonus() && m_bonusDetonator != nullptr)
	{
		bonusList.push_front(EBonus::Detonator);
		bonusList.push_front(EBonus::Detonator);
	}
	if (!_gameInstance->HasVestBonus() && m_bonusVest != nullptr)
	{
		bonusList.push_front(EBonus::Vest);
	}
	if (!_gameInstance->HasGhostWallsBonus() && m_bonusGhostWalls != nullptr)
	{
		bonusList.push_front(EBonus::GhostWalls);
	}
	return bonusList;
}

void ADamageableActor::SpawnBonus(EBonus bonus)
{
	switch (bonus)
	{
		default:
			break;
		case EBonus::Limit:
		{
			AActor* bonusActor = GetWorld()->SpawnActor<AActor>(m_bonusLimit, GetActorLocation(), GetActorRotation());
			ABonus* bonus = Cast<ABonus>(bonusActor);
			if (bonus != nullptr)
			{
				bonus->SetBonusType(0);
			}
			break;
		}
		case EBonus::Power:
		{
			AActor* bonusActor = GetWorld()->SpawnActor<AActor>(m_bonusPower, GetActorLocation(), GetActorRotation());
			ABonus* bonus = Cast<ABonus>(bonusActor);
			if (bonus != nullptr)
			{
				bonus->SetBonusType(1);
			}
			break;
		}
		case EBonus::Speed:
		{
			AActor* bonusActor = GetWorld()->SpawnActor<AActor>(m_bonusSpeed, GetActorLocation(), GetActorRotation());
			ABonus* bonus = Cast<ABonus>(bonusActor);
			if (bonus != nullptr)
			{
				bonus->SetBonusType(2);
			}
			break;
		}
		case EBonus::Detonator:
		{
			AActor* bonusActor = GetWorld()->SpawnActor<AActor>(m_bonusDetonator, GetActorLocation(), GetActorRotation());
			ABonus* bonus = Cast<ABonus>(bonusActor);
			if (bonus != nullptr)
			{
				bonus->SetBonusType(3);
			}
			break;
		}
		case EBonus::Vest:
		{
			AActor* bonusActor = GetWorld()->SpawnActor<AActor>(m_bonusVest, GetActorLocation(), GetActorRotation());
			ABonus* bonus = Cast<ABonus>(bonusActor);
			if (bonus != nullptr)
			{
				bonus->SetBonusType(4);
			}
			break;
		}
		case EBonus::GhostWalls:
		{
			AActor* bonusActor = GetWorld()->SpawnActor<AActor>(m_bonusGhostWalls, GetActorLocation(), GetActorRotation());
			ABonus* bonus = Cast<ABonus>(bonusActor);
			if (bonus != nullptr)
			{
				bonus->SetBonusType(5);
			}
			break;
		}
	}
}

bool ADamageableActor::Damage()
{
	m_health--;
	if (m_health <= 0 && m_isDestroyable)
	{
		ACustomGameMode* gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
		UCustomGameInstance* gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (gameMode != nullptr && gameInstance != nullptr)
		{
			gameMode->RemoveWall(this);
			int32 random = std::rand() % 100;
			if ((random >= 95 || gameMode->GetNbrOfWalls() <= 0) && !gameMode->HasExitSpawned())
			{
				if (m_exit != nullptr)
				{
					GetWorld()->SpawnActor<AActor>(m_exit, GetActorLocation(), GetActorRotation());
					gameMode->SetExitSpawned(true);
				}
			}
			else if ((random <= 5 || gameMode->GetNbrOfWalls() <= gameMode->GetRemainingBonuses()) && gameMode->CanSpawnBonus())
			{
				std::list<EBonus> bonusList = GetAvailablebonusWithWeight(gameInstance);
				if (bonusList.size() != 0)
				{
					std::list<EBonus>::iterator it = bonusList.begin();
					std::advance(it, std::rand() % bonusList.size());
					EBonus bonus = *it;

					SpawnBonus(bonus);
					gameMode->AddSpawnedBonus(1);
				}
			}
		}
		
		Destroy();
		return true;
	}
	else if (m_mesh != nullptr && m_destroyedBricks != nullptr)
	{
		m_mesh->SetStaticMesh(m_destroyedBricks);
	}
	return false;
}
