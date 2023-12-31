#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <stdlib.h>
#include <list>
#include "Bonus.h"
#include <Bomberman/Game/CustomGameInstance.h>
#include "DamageableActor.generated.h"

UCLASS()
class BOMBERMAN_API ADamageableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamageableActor();
	bool Damage();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool m_isDestroyable = false;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 m_sartingHealth = 2;
	int32 m_health = 0;
	UStaticMeshComponent* m_mesh = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMesh* m_destroyedBricks = nullptr;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_bonusPower = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_bonusLimit = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_bonusSpeed = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_bonusDetonator = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_bonusVest = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_bonusGhostWalls = nullptr;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_exit = nullptr;

	std::list<EBonus> GetAvailableBonusWithWeight(UCustomGameInstance* _gameInstance);
	void SpawnBonus(EBonus bonus);
};
