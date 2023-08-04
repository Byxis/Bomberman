#include "ExitAnimInstance.h"
#include <Kismet/GameplayStatics.h>

void UExitAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
}

void UExitAnimInstance::NativeUpdateAnimation(float _deltaSeconds)
{
    Super::NativeUpdateAnimation(_deltaSeconds);

    if (m_exit != nullptr)
    {
        UpdateAnimationProperties();
    }
    else
    {
        TArray<AActor*> actors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExitScript::StaticClass(), actors);

        for (AActor* actor : actors)
        {
            m_exit = Cast<AExitScript>(actor);
            if (m_exit != nullptr)
            {
                UpdateAnimationProperties();
                break;
            }
        }
    }
}

void UExitAnimInstance::UpdateAnimationProperties()
{
    m_isOpened = m_exit->IsOpened();
}
