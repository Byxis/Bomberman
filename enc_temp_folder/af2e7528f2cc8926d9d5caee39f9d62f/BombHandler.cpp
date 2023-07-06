#include "BombHandler.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include <Bomberman/PlayerManager/PlayerControl.h>
#include "./Bomberman/Addons/DamageableActor.h"
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
		m_collision->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		m_collision->SetHiddenInGame(false);
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
		float scaleFactor = FMath::Lerp(0.7f, 1.1f, redValue);
		m_sphere->SetWorldScale3D(FVector(scaleFactor));
	}
	
	if (m_timer <= 0)
	{
		ApplyDestroyEffect();
		m_player->GiveBackBomb();
		m_player->SetCanPlaceBomb(true);
		m_owner->Destroy();
	}
}

void UBombHandler::ApplyDestroyEffect()
{
	FVector startTrace = GetOwner()->GetActorLocation();
	FVector endTrace = FVector(0, 0, 0);

	for (int i = 1; i <= m_power; i++)
	{
		endTrace = FVector(0, 100.0f * i, 0) + startTrace;
		if (CheckAndDoDestruction(startTrace, endTrace))
		{
			break;
		}
	}

	for (int i = 1; i <= m_power; i++)
	{
		endTrace = FVector(0, -100.0f * i, 0) + startTrace;
		if (CheckAndDoDestruction(startTrace, endTrace))
		{
			break;
		}
	}

	for (int i = 1; i <= m_power; i++)
	{
		endTrace = FVector(-100.0f * i, 0, 0) + startTrace;
		if (CheckAndDoDestruction(startTrace, endTrace))
		{
			break;
		}
	}

	for (int i = 1; i <= m_power; i++)
	{
		endTrace = FVector(100.0f * i, 0, 0) + startTrace;
		if (CheckAndDoDestruction(startTrace, endTrace))
		{
			break;
		}
	}
}

bool UBombHandler::CheckAndDoDestruction(FVector _startTrace, FVector _endTrace)
{
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	TArray<FHitResult> hitResults;
	bool destroyedWall = false;

	DrawDebugLine(GetWorld(), _startTrace, _endTrace, FColor(0, 255, 0), true);
	if (GetWorld()->LineTraceMultiByChannel(hitResults, _startTrace, _endTrace, ECC_Visibility, *CQP))
	{
		for (FHitResult& hitResult : hitResults)
		{
			AActor* actor = hitResult.GetActor();
			ADamageableActor* damageableActor = Cast<ADamageableActor>(actor);
			APlayerControl* playerControl = Cast<APlayerControl>(actor);
			AEnemyHandler* enemyHandler = Cast<AEnemyHandler>(actor);

			if (playerControl != nullptr)
			{
				playerControl->Damage();
			}
			if (enemyHandler != nullptr)
			{
				enemyHandler->Damage();
			}
			else if (damageableActor != nullptr)
			{
				damageableActor->Damage();
				destroyedWall = true;
			}
		}
	}
	return destroyedWall;
}

void UBombHandler::OnPlayerLeaveBomb(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (m_sphere != nullptr)
	{
		APlayerControl* playerControl = Cast<APlayerControl>(OtherActor);

		if (playerControl != nullptr)
		{
			playerControl->SetCanPlaceBomb(true);
			m_sphere->SetCollisionProfileName(FName("InvisibleWall"), true);
		}
	}
}