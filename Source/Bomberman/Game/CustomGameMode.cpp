#include "CustomGameMode.h"
#include "CustomGameState.h"
#include "./Bomberman/PlayerManager/PlayerControl.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include <Bomberman/Addons/DamageableActor.h>

ACustomGameMode::ACustomGameMode()
{
	GameStateClass = ACustomGameState::StaticClass();
	levels.Add("Level_1");
	levels.Add("Level_2");
	levels.Add("Level_3");
}

void ACustomGameMode::BeginPlay()
{
	Super::BeginPlay();
	SetCurrentGameState(EGameState::Playing);
	CountWalls();
}

void ACustomGameMode::NextLevel()
{
	GetGameState<ACustomGameState>()->SetLevel(GetGameState<ACustomGameState>()->GetLevel() + 1);
	FString current = GetWorld()->GetMapName();
	current.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	for (int i = 0; i < levels.Num(); i++)
	{
		if (current == levels[i])
		{
			if (i <= levels.Num() - 2)
			{
				UGameplayStatics::OpenLevel(GetWorld(), FName(levels[i + 1]));
				CountWalls();
				SetExitSpawned(false);
			}
		}
	}
}

int32 ACustomGameMode::GetNbrOfWalls()
{
	FString message = FString::Printf(TEXT("Walls: %f"), m_nbrWalls);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, message);
	return m_nbrWalls;
}

void ACustomGameMode::RemoveWall()
{
	m_nbrWalls -= 1;
}

void ACustomGameMode::SetCurrentGameState(EGameState _newState)
{
	m_currentGameState = _newState;
}

EGameState ACustomGameMode::GetCurrentGameState() const
{
	return m_currentGameState;
}
void ACustomGameMode::CountWalls()
{
	TArray<AActor*> walls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADamageableActor::StaticClass(), walls);
	int count = 0;
	for (AActor* wall : walls)
	{
		count += 1;
	}
	m_nbrWalls = count;
}

bool ACustomGameMode::HasExitSpawned()
{
	return m_hasExitSpawned;
}
void ACustomGameMode::SetExitSpawned(bool _bool)
{
	m_hasExitSpawned = _bool;
}

void ACustomGameMode::AddScore(int32 _score)
{
	GetGameState<ACustomGameState>()->AddScore(_score);
	APlayerControl* playerControl =	Cast<APlayerControl>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (playerControl != nullptr)
	{
		playerControl->ActualiseScore(GetScore());
	}
}

int32 ACustomGameMode::GetScore()
{
	return GetGameState<ACustomGameState>()->GetScore();
}
