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
	m_health = m_sartingHealth;
	SetActorTickEnabled(false);

	m_mesh = GetComponentByClass<UStaticMeshComponent>();
	if (m_mesh != nullptr)
	{
		m_materialInstance = m_mesh->CreateDynamicMaterialInstance(0, m_mesh->GetMaterial(0));
		m_materialInstance->SetScalarParameterValue(FName("DamageValue"), m_health);
		m_mesh->GetStaticMesh()->SetMaterial(0, m_materialInstance);
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
			else if (random <= 5 && m_bonus != nullptr)
			{
				AActor* bonusActor = GetWorld()->SpawnActor<AActor>(m_bonus, GetActorLocation(), GetActorRotation());
				ABonus* bonus = Cast<ABonus>(bonusActor);
				if (bonus != nullptr)
				{
					bonus->SetBonusType(random);
				}
			}
		}
		
		Destroy();
		return true;
	}
	else if (m_mesh != nullptr)
	{
		m_materialInstance->SetScalarParameterValue(FName("DamageValue"), m_health);
		m_mesh->GetStaticMesh()->SetMaterial(0, m_materialInstance);
	}
	return false;
}
