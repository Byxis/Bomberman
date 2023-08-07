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
	check(m_mesh);
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

std::list<EBonus> ADamageableActor::GetAvailableBonusWithWeight(UCustomGameInstance* _gameInstance)
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
			AActor* bonusLimitActor = GetWorld()->SpawnActor<AActor>(m_bonusLimit, GetActorLocation() - FVector(0,0,50), GetActorRotation());
			ABonus* bonusLimit = Cast<ABonus>(bonusLimitActor);
			if (bonusLimit != nullptr)
				bonusLimit->SetBonusType(EBonus::Limit);
			break;
		}
		case EBonus::Power:
		{
			AActor* bonusPowerActor = GetWorld()->SpawnActor<AActor>(m_bonusPower, GetActorLocation() - FVector(0, 0, 50), GetActorRotation());
			ABonus* bonusPower = Cast<ABonus>(bonusPowerActor);
			if (bonusPower != nullptr)
				bonusPower->SetBonusType(EBonus::Power);
			break;
		}
		case EBonus::Speed:
		{
			AActor* bonusSpeedActor = GetWorld()->SpawnActor<AActor>(m_bonusSpeed, GetActorLocation() - FVector(0, 0, 50), GetActorRotation());
			ABonus* bonusSpeed = Cast<ABonus>(bonusSpeedActor);
			if (bonusSpeed != nullptr)
				bonusSpeed->SetBonusType(EBonus::Speed);
			break;
		}
		case EBonus::Detonator:
		{
			AActor* bonusDetonatorActor = GetWorld()->SpawnActor<AActor>(m_bonusDetonator, GetActorLocation() - FVector(0, 0, 50), GetActorRotation());
			ABonus* bonusDetonator = Cast<ABonus>(bonusDetonatorActor);
			if (bonusDetonator != nullptr)
				bonusDetonator->SetBonusType(EBonus::Detonator);
			break;
		}
		case EBonus::Vest:
		{
			AActor* bonusVestActor = GetWorld()->SpawnActor<AActor>(m_bonusVest, GetActorLocation() - FVector(0, 0, 50), GetActorRotation());
			ABonus* bonusVest = Cast<ABonus>(bonusVestActor);
			if (bonusVest != nullptr)
				bonusVest->SetBonusType(EBonus::Vest);
			break;
		}
		case EBonus::GhostWalls:
		{
			AActor* bonusGhostWallsActor = GetWorld()->SpawnActor<AActor>(m_bonusGhostWalls, GetActorLocation() - FVector(0, 0, 50), GetActorRotation());
			ABonus* bonusGhostWalls = Cast<ABonus>(bonusGhostWallsActor);
			if (bonusGhostWalls != nullptr)
				bonusGhostWalls->SetBonusType(EBonus::GhostWalls);
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
				std::list<EBonus> bonusList = GetAvailableBonusWithWeight(gameInstance);
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
