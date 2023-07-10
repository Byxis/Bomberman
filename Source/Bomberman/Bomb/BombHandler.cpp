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

UBombHandler::UBombHandler(const FObjectInitializer& _objectInitializer)
	:Super(_objectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	m_owner = GetOwner();
	m_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));
	if (m_collision != nullptr && m_owner != nullptr)
	{
		m_collision->SetupAttachment(m_owner->GetRootComponent());
		m_collision->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
		m_collision->SetHiddenInGame(true);
		m_collision->OnComponentEndOverlap.AddDynamic(this, &UBombHandler::OnPlayerLeaveBomb);
	}
}

void UBombHandler::SetPower(int32 _newPower)
{
	m_power = _newPower;
}


void UBombHandler::BeginPlay()
{
	Super::BeginPlay();
	
	m_player = Cast<APlayerControl>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	check(m_player != nullptr);
	check(m_owner != nullptr);
	if (m_owner != nullptr)
	{
		m_sphere = Cast<UStaticMeshComponent>(m_owner->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		m_owner->SetActorTickEnabled(false);
	}

	if (m_sphere != nullptr)
	{
		m_material = m_sphere->CreateDynamicMaterialInstance(0, m_sphere->GetMaterial(0));
		m_sphere->SetCollisionProfileName(FName("Trigger"), true);
	}

	if (m_player != nullptr && m_owner != nullptr)
	{
		m_isInitalisationDone = true;
		m_owner->SetActorTickEnabled(true);
	}
}

void UBombHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	m_timer -= DeltaTime;

	if (m_material != nullptr)
	{
		float redValue = (FMath::Sin(GetWorld()->GetTimeSeconds() * 2.0f * PI) + 1.0f) / 2.0f;
		m_material->SetVectorParameterValue(FName("AdditiveColor"), FLinearColor(redValue, 0.0f, 0.0f));
		m_sphere->GetStaticMesh()->SetMaterial(0, m_material);
		float scaleFactor = FMath::Lerp(0.4f, 0.6f, redValue);
		m_sphere->SetWorldScale3D(FVector(scaleFactor));
	}
	
	if (m_timer <= 0)
	{
		Explode();
	}
}

void UBombHandler::Explode()
{
	m_exploding = true;
	ApplyDestroyEffect();
	m_player->GiveBackBomb();
	if (m_sphere->GetCollisionProfileName().IsEqual(FName("Trigger")))
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
		UE_LOG(LogTemp, Warning, TEXT("IN"))
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
	bool result = true;

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
				ADamageableActor* damageableActor = Cast<ADamageableActor>(actor);

				if (playerControl == nullptr && enemyHandler == nullptr && bonus == nullptr && bomb == nullptr)
				{
					result = false;
				}
			}
		}
	}
	return result;
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

				if (damageableActor != nullptr)
				{
					if (damageableActor->Damage())
					{
						GetWorld()->SpawnActor<AActor>(m_flames, _endTrace, FRotator(0, 0, 0));
					}
				}
			}
		}
	}
}

void UBombHandler::OnPlayerLeaveBomb(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (m_sphere != nullptr)
	{
		APlayerControl* playerControl = Cast<APlayerControl>(OtherActor);

		if (playerControl != nullptr)
		{
			playerControl->SetCanPlaceBomb(true);
			m_sphere->SetCollisionProfileName(FName("InvisibleWallDynamic"), true);
		}
	}
}

bool UBombHandler::IsExploding()
{
	return m_exploding;
}
