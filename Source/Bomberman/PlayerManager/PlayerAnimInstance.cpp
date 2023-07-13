#include "PlayerAnimInstance.h"
#include <Kismet/GameplayStatics.h>

void UPlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    m_player = Cast<APlayerControl>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float _deltaSeconds)
{
    Super::NativeUpdateAnimation(_deltaSeconds);

    if (m_player != nullptr)
    {
        UpdateAnimationProperties();
    }
    else
    {
        TArray<AActor*> actors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerControl::StaticClass(), actors);

        for (AActor* actor : actors)
        {
            m_player = Cast<APlayerControl>(actor);
            if (m_player != nullptr)
            {
                UpdateAnimationProperties();
                break;
            }
        }
    }
}

void UPlayerAnimInstance::UpdateAnimationProperties()
{
    m_speed = m_player->GetVelocity().Size()*2;
    m_isDead = m_player->IsDead();
    m_isPlacingBomb = m_player->IsPlacingBomb();
    if (m_isPlacingBomb)
        m_player->SetPlacingBomb(false);
}
