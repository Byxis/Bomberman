#include "DamageableActor.h"

ADamageableActor::ADamageableActor()
{
}

void ADamageableActor::BeginPlay()
{
	Super::BeginPlay();
	m_health = m_maxHealth;
	SetActorTickEnabled(false);
}

void ADamageableActor::Damage()
{
	m_health--;
	if (m_health <= 0 && m_isDestroyable)
	{
		FString message = FString::Printf(TEXT("Wall Destroyed"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, message);
		Destroy();
	}
}
