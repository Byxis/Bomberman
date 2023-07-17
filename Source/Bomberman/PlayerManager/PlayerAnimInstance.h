#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerControl.h"
#include "PlayerAnimInstance.generated.h"

UCLASS()
class BOMBERMAN_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float _deltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Player")
	float m_speed = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Player")
	bool m_isDead = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Player")
	bool m_isPlacingBomb = false;
	

private:
	void UpdateAnimationProperties();

	APlayerControl* m_player = nullptr;
};
