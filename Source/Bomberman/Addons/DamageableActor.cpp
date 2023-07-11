#include "DamageableActor.h"
#include "GameFramework/Character.h"
#include <Bomberman/Game/CustomGameMode.h>
#include "Bonus.h"

ADamageableActor::ADamageableActor()
{

}

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
}

bool ADamageableActor::Damage()
{
	m_health--;
	if (m_health <= 0 && m_isDestroyable)
	{
		ACustomGameMode* GameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
		if (GameMode != nullptr)
		{
			GameMode->RemoveWall();
			int32 random = std::rand() % 100;
			if (random >= 95 || GameMode->GetNbrOfWalls() <= 0)
			{
				if (!GameMode->HasExitSpawned() && m_exit != nullptr)
				{
					GetWorld()->SpawnActor<AActor>(m_exit, GetActorLocation(), GetActorRotation());
					GameMode->SetExitSpawned(true);
				}
			}
			else if (random <= 5)
			{
				random = std::rand() % 100;
				if (random <= 49 && m_bonusLimit != nullptr)
				{
					AActor* bonusActor = GetWorld()->SpawnActor<AActor>(m_bonusLimit, GetActorLocation(), GetActorRotation());
					ABonus* bonus = Cast<ABonus>(bonusActor);
					if (bonus != nullptr)
					{
						bonus->SetBonusType(0);
					}
				}
				else if (random >= 50 && m_bonusPower != nullptr)
				{
					AActor* bonusActor = GetWorld()->SpawnActor<AActor>(m_bonusPower, GetActorLocation(), GetActorRotation());
					ABonus* bonus = Cast<ABonus>(bonusActor);
					if (bonus != nullptr)
					{
						bonus->SetBonusType(1);
					}
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
