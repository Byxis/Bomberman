#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Bomberman/Game/CustomGameMode.h>
#include "EnemySpawner.generated.h"

UCLASS()
class BOMBERMAN_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();
	virtual void Tick(float DeltaTime) override;
	void SpawnEnemies(bool isBonusLevel);

protected:
	virtual void BeginPlay() override;

private:	
	bool m_isBonusLevel = false;
	ACustomGameMode* m_gameMode = nullptr;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_enemy = nullptr;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FVector> m_spawnLocations;
};
