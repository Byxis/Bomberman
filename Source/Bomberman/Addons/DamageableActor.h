#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageableActor.generated.h"

UCLASS()
class BOMBERMAN_API ADamageableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamageableActor();
	void Damage();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool m_isDestroyable = false;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 m_maxHealth = 2;
	int32 m_health = 0;
};
