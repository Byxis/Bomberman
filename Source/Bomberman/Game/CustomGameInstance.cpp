#include "CustomGameInstance.h"
#include <Bomberman/PlayerManager/PlayerControl.h>
#include <Kismet/GameplayStatics.h>

UCustomGameInstance::UCustomGameInstance()
{
}

void UCustomGameInstance::Init()
{
    Super::Init();
    m_bombLimit = 1;
    m_bombPower = 1;
    m_score = 0;
}

int32 UCustomGameInstance::GetBombLimit()
{
    return m_bombLimit;
}

void UCustomGameInstance::AddBombLimit(int32 _bombLimit)
{
    m_bombLimit += _bombLimit;
}

bool UCustomGameInstance::HasMaxedBombLimit()
{
    return m_bombLimit >= m_maxBombLimit;
}

int32 UCustomGameInstance::GetBombPower()
{
    return m_bombPower;
}

void UCustomGameInstance::AddBombPower(int32 _bombPower)
{
    m_bombPower += _bombPower;
}

bool UCustomGameInstance::HasMaxedBombPower()
{
    return m_bombPower >= m_maxBombPower;
}

int32 UCustomGameInstance::GetScore()
{
    return m_score;
}

void UCustomGameInstance::SetScore(int32 _score)
{
    m_score = _score;
    APlayerControl* playerControl = Cast<APlayerControl>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (playerControl != nullptr)
    {
        playerControl->ActualiseScore();
    }
}

void UCustomGameInstance::AddScore(int32 _score)
{
    m_score += _score;
    APlayerControl* playerControl = Cast<APlayerControl>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (playerControl != nullptr)
    {
        playerControl->ActualiseScore();
    }
}

float UCustomGameInstance::GetSpeed()
{
    return m_speed;
}

void UCustomGameInstance::AddSpeed(float _speed)
{
    m_speed += _speed;
}

bool UCustomGameInstance::HasMaxedSpeed()
{
    return m_speed >= m_maxSpeed;
}

bool UCustomGameInstance::HasDetonatorBonus()
{
    return m_detonator;
}

void UCustomGameInstance::SetDetonatorBonus(bool _detonator)
{
    m_detonator = _detonator;
}
