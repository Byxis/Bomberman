#include "EnemyHandler.h"
#include <Bomberman/PlayerManager/PlayerControl.h>
#include <Bomberman/Bomb/BombHandler.h>
#include <list>
#include <iostream>
#include <stdlib.h>

AEnemyHandler::AEnemyHandler(const FObjectInitializer& _objectInitializer)
	:Super(_objectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	m_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));

	if (m_collision != nullptr)
	{
		m_collision->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.0f));
		m_collision->SetHiddenInGame(false);
		m_collision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyHandler::OnInteract);

		RootComponent = m_collision;
	}
}

// Called when the game starts or when spawned
void AEnemyHandler::BeginPlay()
{
	Super::BeginPlay();
	ChangeDirection();
}


void AEnemyHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector position = GetActorLocation();
	if (m_timer > 0)
	{
		m_timer -= DeltaTime;
	}
	if (m_currentDirection == EDirection::North)
	{
		position.X -= m_speed * DeltaTime;
		if (std::fmod(position.X, 200) < 1 && m_timer <= 0)
		{
			ChangeDirection();
		}
	}
	else if (m_currentDirection == EDirection::South)
	{
		position.X += m_speed * DeltaTime;
		if (std::fmod(position.X, 200) < 1 && m_timer <= 0)
		{
			ChangeDirection();
		}
	}
	else if (m_currentDirection == EDirection::West)
	{
		position.Y -= m_speed * DeltaTime;
		if (std::fmod(position.Y, 200) < 1 && m_timer <= 0)
		{
			ChangeDirection();
		}
	}
	else if (m_currentDirection == EDirection::East)
	{
		position.Y += m_speed * DeltaTime;
		if (std::fmod(position.Y, 200) < 1 && m_timer <= 0)
		{
			ChangeDirection();
		}
	}
	SetActorLocation(position);
}

void AEnemyHandler::Damage()
{
	Destroy();
}

void AEnemyHandler::OnInteract(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerControl* playerControl = Cast<APlayerControl>(OtherActor);
	UBombHandler* bomb = Cast<UBombHandler>(OtherActor);

	if (playerControl != nullptr)
	{
		playerControl->Damage();
	}
	ChangeDirection();
}

void AEnemyHandler::ChangeDirection()
{
	std::list<EDirection> directions;
	if (IsDirectionAvailable(FVector(-100, 0, 0)))
	{
		directions.push_front(EDirection::North);
	}
	if (IsDirectionAvailable(FVector(100, 0, 0)))
	{
		directions.push_front(EDirection::South);
	}
	if (IsDirectionAvailable(FVector(0, -100, 0)))
	{
		directions.push_front(EDirection::West);
	}
	if (IsDirectionAvailable(FVector(0, 100, 0)))
	{
		directions.push_front(EDirection::East);
	}

	if (directions.size() == 0)
	{
		m_currentDirection = EDirection::None;
	}
	else
	{
		std::list<EDirection>::iterator it = directions.begin();
		std::advance(it, std::rand() % directions.size());
		m_currentDirection = *it;
	}
}

bool AEnemyHandler::IsDirectionAvailable(FVector _direction)
{
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	FHitResult* hitResult = new FHitResult();
	if (GetWorld()->LineTraceSingleByChannel(*hitResult, GetActorLocation(), GetActorLocation()+_direction, ECC_Visibility, *CQP))
	{
		if (hitResult->GetActor() != nullptr)
		{
			return false;
		}
	}
	return false;
}