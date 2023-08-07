#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ExitScript.h"
#include "ExitAnimInstance.generated.h"

UCLASS()
class BOMBERMAN_API UExitAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float _deltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	bool m_isOpened = false;

private:
	void UpdateAnimationProperties();

	AExitScript* m_exit = nullptr;
};
