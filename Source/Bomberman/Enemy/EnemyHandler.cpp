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
		m_collision->SetHiddenInGame(true);
		m_collision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyHandler::OnInteract);

		RootComponent = m_collision;
	}
}

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
		if (std::fmod(position.X+50, 100) < 10)
		{
			ChangeDirection();
		}
	}
	else if (m_currentDirection == EDirection::South)
	{
		position.X += m_speed * DeltaTime;
		if (std::fmod(position.X + 50, 100) < 10)
		{
			ChangeDirection();
		}
	}
	else if (m_currentDirection == EDirection::West)
	{
		position.Y -= m_speed * DeltaTime;
		if (std::fmod(position.Y + 50, 100) < 10)
		{
			ChangeDirection();
		}
	}
	else if (m_currentDirection == EDirection::East)
	{
		position.Y += m_speed * DeltaTime;
		if (std::fmod(position.Y + 50, 100) < 10)
		{
			ChangeDirection();
		}
	}
	else if (m_currentDirection == EDirection::East)
	{
		ChangeDirection();
	}
	SetActorLocation(position);
}

void AEnemyHandler::Damage()
{
	Destroy();
}

void AEnemyHandler::OnInteract(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("COLLISION"))
	APlayerControl* playerControl = Cast<APlayerControl>(OtherActor);
	UBombHandler* bomb = Cast<UBombHandler>(OtherActor);

	if (playerControl != nullptr)
	{
		playerControl->Damage();
	}
	else if (bomb != nullptr)
	{
		ChangeDirection();
	}
}

void AEnemyHandler::ChangeDirection()
{
	std::list<EDirection> directions;
	if (IsDirectionAvailable(FVector(-130, 0, 0)))
	{
		directions.push_front(EDirection::North);
		if (m_currentDirection == EDirection::North && m_timer > 0)
		{
			return;
		}
	}
	if (IsDirectionAvailable(FVector(130, 0, 0)))
	{
		directions.push_front(EDirection::South);
		if (m_currentDirection == EDirection::South && m_timer > 0)
		{
			return;
		}
	}
	if (IsDirectionAvailable(FVector(0, -130, 0)))
	{
		directions.push_front(EDirection::West);
		if (m_currentDirection == EDirection::West && m_timer > 0)
		{
			return;
		}
	}
	if (IsDirectionAvailable(FVector(0, 130, 0)))
	{
		directions.push_front(EDirection::East);
		if (m_currentDirection == EDirection::East && m_timer > 0)
		{
			return;
		}
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
		m_timer = 2.0f;
	}
}

bool AEnemyHandler::IsDirectionAvailable(FVector _direction)
{
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	CQP->AddIgnoredActor(this);
	FHitResult* hitResult = new FHitResult();
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + _direction, FColor(0, 255, 0), false, 2.0f);
	if (GetWorld()->LineTraceSingleByChannel(*hitResult, GetActorLocation(), GetActorLocation()+_direction, ECC_Visibility, *CQP))
	{
		AActor* actor = hitResult->GetActor();
		if (actor != nullptr)
		{
			APlayerControl* playerControl = Cast<APlayerControl>(actor);
			if (playerControl != nullptr)
				return true;
			return false;
		}
	}
	return true;
}