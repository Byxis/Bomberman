#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include <Bomberman/Game/CustomGameMode.h>
#include "EnemyHandler.generated.h"

UCLASS()
class BOMBERMAN_API AEnemyHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyHandler(const FObjectInitializer& _objectInitializer);
	virtual void Tick(float DeltaTime) override;
	void Damage();
	UFUNCTION()
	void OnInteract(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

private:
	enum class EDirection : int8
	{
		North,
		South,
		East,
		West,
		None
	};
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float m_speedMultiplier = 1.0f;
	float m_speed = 200.0f;
	UBoxComponent* m_collision = nullptr;
	EDirection m_currentDirection = EDirection::South;
	float m_defaultTimer = 2;
	float m_timer = m_defaultTimer;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int m_score = 100;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool m_iscloudEnemy = false;
	AActor* m_playerPawn = nullptr;
	ACustomGameMode* m_gameMode = nullptr;

	void ChangeDirection();
	bool IsDirectionAvailable(FVector _direction);
};
