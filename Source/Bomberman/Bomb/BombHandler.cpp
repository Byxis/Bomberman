#include "BombHandler.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include <Bomberman/PlayerManager/PlayerControl.h>
#include "./Bomberman/Addons/DamageableActor.h"
#include <Bomberman/Addons/Bonus.h>
#include "Engine/World.h"
#include <Bomberman/Enemy/EnemyHandler.h>
#include <Bomberman/Game/CustomGameMode.h>
#include "Flames.h"

UBombHandler::UBombHandler(const FObjectInitializer& _objectInitializer)
	:Super(_objectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	m_owner = GetOwner();
	m_collisionExit = CreateDefaultSubobject<UBoxComponent>(TEXT("collisionExit"));
	if (m_collisionExit != nullptr && m_owner != nullptr)
	{
		m_collisionExit->SetupAttachment(m_owner->GetRootComponent());
		m_collisionExit->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
		m_collisionExit->SetCollisionProfileName(FName("GhostBomb"), true);
		m_collisionExit->OnComponentEndOverlap.AddDynamic(this, &UBombHandler::OnPlayerLeaveBomb);
	}
	m_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));
	if (m_collision != nullptr && m_owner != nullptr)
	{
		m_collision->SetupAttachment(m_owner->GetRootComponent());
		m_collision->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	}
}

void UBombHandler::SetPower(int32 _newPower)
{
	m_power = _newPower;
}

void UBombHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!m_isInitalisationDone)
	{
		TryInit();
		return;
	}
		
	m_timer -= DeltaTime;

	if (m_material != nullptr)
	{
		float redValue = (FMath::Sin(GetWorld()->GetTimeSeconds() * 2.0f * PI) + 1.0f) / 2.0f;
		m_material->SetVectorParameterValue(FName("AdditiveColor"), FLinearColor(redValue, 0.0f, 0.0f));
		m_skeleton->SetMaterial(0, m_material);
	}
	
	if (m_timer <= 0)
	{
		Explode();
	}
}

void UBombHandler::Explode()
{
	if (m_explosionSFX != nullptr)
	{
		m_explosionAudio = UGameplayStatics::SpawnSound2D(GetWorld(), m_explosionSFX, 1, 1, 0);
	}

	ACustomGameMode* gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
	m_exploding = true;

	if (gameMode != nullptr)
	{
		gameMode->RemoveBomb(this);
	}

	ApplyDestroyEffect();
	m_player->GiveBackBomb();

	if (m_collision->GetCollisionProfileName().IsEqual(FName("GhostBomb")))
	{
		m_player->SetCanPlaceBomb(true);
	}
	m_owner->Destroy();
}

void UBombHandler::ApplyDestroyEffect()
{
	FVector startTrace = GetOwner()->GetActorLocation();
	FVector endTrace = FVector(0, 0, 0);

	GetWorld()->SpawnActor<AActor>(m_flames, startTrace, FRotator(0, 0, 0));
	for (int i = 1; i <= m_power; i++)
	{
		endTrace = FVector(0, 100.0f, 0) + startTrace;
		if (!CanSpawnFlames(startTrace, endTrace))
		{
			CheckIfWall(startTrace, endTrace);
			break;
		}
		GetWorld()->SpawnActor<AActor>(m_flames, endTrace, FRotator(0, 0, 0));
		startTrace = endTrace;
	}

	startTrace = GetOwner()->GetActorLocation();
	endTrace = FVector(0, 0, 0);

	for (int i = 1; i <= m_power; i++)
	{
		endTrace = FVector(0, -100.0f, 0) + startTrace;
		if (!CanSpawnFlames(startTrace, endTrace))
		{
			CheckIfWall(startTrace, endTrace);
			break;
		}
		GetWorld()->SpawnActor<AActor>(m_flames, endTrace, FRotator(0, 0, 0));
		startTrace = endTrace;
	}

	startTrace = GetOwner()->GetActorLocation();
	endTrace = FVector(0, 0, 0);

	for (int i = 1; i <= m_power; i++)
	{
		endTrace = FVector(-100.0f, 0, 0) + startTrace;
		if (!CanSpawnFlames(startTrace, endTrace))
		{
			CheckIfWall(startTrace, endTrace);
			break;
		}
		GetWorld()->SpawnActor<AActor>(m_flames, endTrace, FRotator(0, 0, 0));
		startTrace = endTrace;
	}

	startTrace = GetOwner()->GetActorLocation();
	endTrace = FVector(0, 0, 0);

	for (int i = 1; i <= m_power; i++)
	{
		endTrace = FVector(100.0f, 0, 0) + startTrace;
		if (!CanSpawnFlames(startTrace, endTrace))
		{
			CheckIfWall(startTrace, endTrace);
			break;
		}
		GetWorld()->SpawnActor<AActor>(m_flames, endTrace, FRotator(0, 0, 0));
		startTrace = endTrace;
	}
}

bool UBombHandler::CanSpawnFlames(FVector _startTrace, FVector _endTrace)
{
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	TArray<FHitResult> hitResults;

	if (GetWorld()->LineTraceMultiByChannel(hitResults, _startTrace, _endTrace, ECC_Visibility, *CQP))
	{
		for (FHitResult& hitResult : hitResults)
		{
			AActor* actor = hitResult.GetActor();
			if (actor != nullptr)
			{
				APlayerControl* playerControl = Cast<APlayerControl>(actor);
				AEnemyHandler* enemyHandler = Cast<AEnemyHandler>(actor);
				ABonus* bonus = Cast<ABonus>(actor);
				UBombHandler* bomb = actor->GetComponentByClass<UBombHandler>();
				AFlames* flames = Cast<AFlames>(actor);

				if (playerControl == nullptr && enemyHandler == nullptr && bonus == nullptr && bomb == nullptr && flames == nullptr)
				{
					return false;
				}
			}
		}
	}
	return true;
}

void UBombHandler::CheckIfWall(FVector _startTrace, FVector _endTrace)
{
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	TArray<FHitResult> hitResults;

	if (GetWorld()->LineTraceMultiByChannel(hitResults, _startTrace, _endTrace, ECC_Visibility, *CQP))
	{
		for (FHitResult& hitResult : hitResults)
		{
			AActor* actor = hitResult.GetActor();
			if (actor != nullptr)
			{
				ADamageableActor* damageableActor = Cast<ADamageableActor>(actor); 
				UBombHandler* bomb = actor->GetComponentByClass<UBombHandler>();

				if (damageableActor != nullptr && damageableActor->Damage())
				{
					GetWorld()->SpawnActor<AActor>(m_flames, _endTrace, FRotator(0, 0, 0));
				}
			}
		}
	}
}

void UBombHandler::TryInit()
{
	m_player = Cast<APlayerControl>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (m_owner != nullptr)
	{
		m_skeleton = Cast<USkeletalMeshComponent>(m_owner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		m_owner->SetActorTickEnabled(false);
	}

	if (m_skeleton != nullptr)
	{
		m_material = m_skeleton->CreateDynamicMaterialInstance(0, m_skeleton->GetMaterial(0));
		m_collision->SetCollisionProfileName(FName("GhostBomb"), true);
	}

	if (m_player != nullptr && m_owner != nullptr)
	{
		m_owner->SetActorTickEnabled(true);
	}

	ACustomGameMode* gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
	if (gameMode != nullptr)
	{
		gameMode->AddBomb(this);
	}

	UCustomGameInstance* gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (m_smokeSFX != nullptr && gameInstance != nullptr)
	{
		m_smokeAudio = UGameplayStatics::SpawnSound2D(GetWorld(), m_smokeSFX, gameInstance->GetSFXVolume(), 1, 0);
	}

	if (m_player != nullptr && m_owner != nullptr && gameMode != nullptr && gameInstance != nullptr)
	{
		m_isInitalisationDone = true;
	}
}

void UBombHandler::OnPlayerLeaveBomb(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (m_collision != nullptr)
	{
		APlayerControl* playerControl = Cast<APlayerControl>(OtherActor);

		if (playerControl != nullptr)
		{
			playerControl->SetCanPlaceBomb(true);
			m_collision->SetCollisionProfileName(FName("Bomb"), true);
		}
	}
}

bool UBombHandler::IsExploding()
{
	return m_exploding;
}

void UBombHandler::PauseSFX(bool _bool)
{
	if (m_explosionAudio != nullptr)
	{
		m_explosionAudio->SetPaused(_bool);
	}
	if (m_smokeAudio != nullptr)
	{
		m_smokeAudio->SetPaused(_bool);
	}
}

void UBombHandler::SetSFXVolume(float _amount)
{
	if (m_explosionAudio != nullptr)
	{
		m_explosionAudio->SetVolumeMultiplier(_amount);
	}
	if (m_smokeAudio != nullptr)
	{
		m_smokeAudio->SetVolumeMultiplier(_amount);
	}
}
