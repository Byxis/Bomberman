#include "EnemyHandler.h"
#include <Bomberman/PlayerManager/PlayerControl.h>
#include <Bomberman/Bomb/BombHandler.h>
#include <list>
#include <iostream>
#include <stdlib.h>
#include <Bomberman/Game/CustomGameInstance.h>
#include <Kismet/GameplayStatics.h>

AEnemyHandler::AEnemyHandler(const FObjectInitializer& _objectInitializer)
	:Super(_objectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	m_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("collision"));
	

	if (m_collision != nullptr)
	{
		m_collision->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.0f));
		if (m_iscloudEnemy)
		{
			m_collision->SetCollisionProfileName(FName("CloudEnemy"));
		}
		else
		{
			m_collision->SetCollisionProfileName(FName("Enemy"));
		}
		m_collision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyHandler::OnInteract);

		RootComponent = m_collision;
	}
}

void AEnemyHandler::BeginPlay()
{
	Super::BeginPlay();
	ChangeDirection();
	m_speed *= m_speedMultiplier;
	m_timer /= m_speedMultiplier;
	ACustomGameMode* gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
	if (gameMode != nullptr)
	{
		gameMode->AddEnemy(this);
	}

	m_playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
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
		position.Y -= m_speed * DeltaTime;
		if (std::fmod(position.Y+50, 100) < 10)
		{
			ChangeDirection();
		}
	}
	else if (m_currentDirection == EDirection::South)
	{
		position.Y += m_speed * DeltaTime;
		if (std::fmod(position.Y + 50, 100) < 10)
		{
			ChangeDirection();
		}
	}
	else if (m_currentDirection == EDirection::West)
	{
		position.X -= m_speed * DeltaTime;
		if (std::fmod(position.X + 50, 100) < 10)
		{
			ChangeDirection();
		}
	}
	else if (m_currentDirection == EDirection::East)
	{
		position.X += m_speed * DeltaTime;
		if (std::fmod(position.X + 50, 100) < 10)
		{
			ChangeDirection();
		}
	}
	else if (m_currentDirection == EDirection::None)
	{
		ChangeDirection();
	}
	SetActorLocation(position);
}

void AEnemyHandler::Damage()
{
	UCustomGameInstance* gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (gameInstance != nullptr)
	{
		gameInstance->AddScore(m_score);
	}
	ACustomGameMode* gameMode = Cast<ACustomGameMode>(UGameplayStatics::GetGameMode(this));
	if (gameMode != nullptr)
	{
		gameMode->RemoveEnemy(this);
	}
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
	else if (bomb != nullptr)
	{
		ChangeDirection();
	}
}

void AEnemyHandler::ChangeDirection()
{
	std::list<EDirection> directions;

	if (IsDirectionAvailable(FVector(0, -100, 0)))
	{
		if (m_currentDirection == EDirection::North && m_timer > 0)
		{
			return;
		}
		if (m_playerPawn != nullptr)
		{
			if (m_playerPawn->GetActorLocation().Y < GetActorLocation().Y)
			{
				directions.push_front(EDirection::North);
				directions.push_front(EDirection::North);
			}
		}

		directions.push_front(EDirection::North);
	}
	if (IsDirectionAvailable(FVector(0, 100, 0)))
	{
		if (m_currentDirection == EDirection::South && m_timer > 0)
		{
			return;
		}
		if (m_playerPawn != nullptr)
		{
			if (m_playerPawn->GetActorLocation().Y > GetActorLocation().Y)
			{
				directions.push_front(EDirection::South);
				directions.push_front(EDirection::South);
			}
		}

		directions.push_front(EDirection::South);
	}
	if (IsDirectionAvailable(FVector(-100, 0, 0)))
	{
		if (m_currentDirection == EDirection::West && m_timer > 0)
		{
			return;
		}
		if (m_playerPawn != nullptr)
		{
			if (m_playerPawn->GetActorLocation().X < GetActorLocation().X)
			{
				directions.push_front(EDirection::West);
				directions.push_front(EDirection::West);
			}
		}

		directions.push_front(EDirection::West);
	}
	if (IsDirectionAvailable(FVector(100, 0, 0)))
	{
		if (m_currentDirection == EDirection::East && m_timer > 0)
		{
			return;
		}
		if (m_playerPawn != nullptr)
		{
			if (m_playerPawn->GetActorLocation().X > GetActorLocation().X)
			{
				directions.push_front(EDirection::East);
				directions.push_front(EDirection::East);
			}
		}

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
		
		m_timer = m_defaultTimer;
		m_timer /= m_speedMultiplier;
	}
}

bool AEnemyHandler::IsDirectionAvailable(FVector _direction)
{
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	CQP->AddIgnoredActor(this);
	TArray<FHitResult> hitResults;

	if (GetWorld()->LineTraceMultiByChannel(hitResults, GetActorLocation(), GetActorLocation()+_direction, ECC_OverlapAll_Deprecated, *CQP))
	{
		for (FHitResult& hitResult : hitResults)
		{
			AActor* actor = hitResult.GetActor();
			if (actor != nullptr)
			{
				APlayerControl* playerControl = Cast<APlayerControl>(actor);
				if (playerControl == nullptr)
					return false;
			}
		}
	}
	return true;
}