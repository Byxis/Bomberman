#include "BombHandler.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include <Bomberman/PlayerManager/PlayerControl.h>
#include "./Bomberman/Addons/DamageableActor.h"

UBombHandler::UBombHandler()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBombHandler::SetPower(int32 _newPower)
{
	m_power = _newPower;
}

void UBombHandler::BeginPlay()
{
	Super::BeginPlay();
	
	m_player = Cast<APlayerControl>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	m_owner = GetOwner();

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
		FString message = FString::Printf(TEXT("got new bomb"));
		m_player->GiveBackBomb();
		ApplyDestroyEffect();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, message);
		m_owner->Destroy();
	}
}

void UBombHandler::ApplyDestroyEffect()
{
	FVector startTrace = GetOwner()->GetActorLocation();
	FVector endTrace = FVector(0.0f, 0.0f, 0.0f);

	for (int i = 1; i <= m_power; i++)
	{
		endTrace = FVector(0.0f, 100.0f * i, 0.0f) + startTrace;
		if (CheckAndDoDestruction(startTrace, endTrace))
		{
			break;
		}
	}

	for (int i = 1; i <= m_power; i++)
	{
		endTrace = FVector(0.0f, -100.0f * i, 0.0f) + startTrace;
		if (CheckAndDoDestruction(startTrace, endTrace))
		{
			break;
		}
	}

	for (int i = 1; i <= m_power; i++)
	{
		endTrace = FVector(-100.0f * i, 0.0f, 0.0f) + startTrace;
		if (CheckAndDoDestruction(startTrace, endTrace))
		{
			break;
		}
	}

	for (int i = 1; i <= m_power; i++)
	{
		endTrace = FVector(100.0f * i, 0.0f, 0.0f) + startTrace;
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

	DrawDebugLine(GetWorld(), _startTrace, _endTrace, FColor(255, 0, 0), true);
	if (GetWorld()->LineTraceMultiByChannel(hitResults, _startTrace, _endTrace, ECC_Visibility, *CQP))
	{
		for (FHitResult& hitResult : hitResults)
		{
			AActor* actor = hitResult.GetActor();
			ADamageableActor* damageableActor = Cast<ADamageableActor>(actor);
			APlayerControl* playerControl = Cast<APlayerControl>(actor);
			if (damageableActor != nullptr)
			{
				damageableActor->Damage();
				destroyedWall = true;
			}
			else if (playerControl != nullptr)
			{
				playerControl->Damage();
			}
		}
	}
	return destroyedWall;
}
