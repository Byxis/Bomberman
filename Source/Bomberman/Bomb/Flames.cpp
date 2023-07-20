#include "Flames.h"
#include <Bomberman/PlayerManager/PlayerControl.h>
#include "NiagaraComponent.h"
#include <Bomberman/Addons/Bonus.h>
#include <Bomberman/Enemy/EnemyHandler.h>
#include "BombHandler.h"

AFlames::AFlames(const FObjectInitializer& _objectInitializer)
	:Super(_objectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	m_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));
	if (m_collision != nullptr)
	{
		m_collision->SetRelativeScale3D(FVector(1.3f, 1.3f, 1.3f));
		m_collision->SetHiddenInGame(true);
		m_collision->SetHiddenInSceneCapture(true);
		m_collision->SetCollisionProfileName(FName("OverlapAll"));
		m_collision->OnComponentBeginOverlap.AddDynamic(this, &AFlames::OnEntityInteract);

		RootComponent = m_collision;
	}
}
void AFlames::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_timer -= DeltaTime;
	if (m_timer <= 1.5f && m_particuleActivated)
	{
		m_particuleActivated = false;
		UNiagaraComponent* particles = GetComponentByClass<UNiagaraComponent>();
		if (particles != nullptr)
			particles->GetSystemInstanceController()->Deactivate(false);
	}
	if (m_timer <= 0)
	{
		Destroy();
	}
}

void AFlames::OnEntityInteract(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (m_timer >= 0.7f)
	{
		ACustomGameMode* gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
		APlayerControl* playerControl = Cast<APlayerControl>(OtherActor);
		if (playerControl != nullptr)
		{
			bool canPlayerTakeDamage = true;
			UCustomGameInstance* gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

			if (gameMode != nullptr)
			{
				canPlayerTakeDamage = !gameMode->IsBonusLevel();
			}

			if (gameInstance != nullptr && canPlayerTakeDamage)
			{
				canPlayerTakeDamage = !gameInstance->HasVestBonus();
			}
			
			if (canPlayerTakeDamage)
			{
				playerControl->Damage();
			}
		}

		ABonus* bonus = Cast<ABonus>(OtherActor);
		if (bonus != nullptr)
		{
			bonus->Damage();
		}

		AEnemyHandler* enemy = Cast<AEnemyHandler>(OtherActor);
		if (enemy != nullptr)
		{
			enemy->Damage();
		}

		UBombHandler* bomb = OtherActor->GetComponentByClass<UBombHandler>();
		if (bomb != nullptr && m_timer == 2.5f)
		{
			if (!bomb->IsExploding() && GetDistanceTo(OtherActor) <= 50)
				bomb->Explode();
		}
	}
	
}

void AFlames::BeginPlay()
{
	Super::BeginPlay();

	if (m_flamesSFX != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), m_flamesSFX, 1, 1, 0);
	}
}
