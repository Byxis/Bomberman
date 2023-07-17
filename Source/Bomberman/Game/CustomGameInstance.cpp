#include "CustomGameInstance.h"
#include <Bomberman/PlayerManager/PlayerControl.h>
#include <Kismet/GameplayStatics.h>

UCustomGameInstance::UCustomGameInstance()
{}

void UCustomGameInstance::Init()
{
    Super::Init();
    m_bombLimit = 1;
    m_bombPower = 1;
    m_score = 0;
    LoadLeaderboard();
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

void UCustomGameInstance::ResetAll()
{
    m_bombLimit = 1;
    m_maxBombLimit = 8;
    m_bombPower = 1;
    m_maxBombPower = 8;
    m_score = 0;
    m_speed = 0.9;
    m_maxSpeed = 2.4;
    m_detonator = false;
}

void UCustomGameInstance::AddToLeaderboard(FString _name)
{
    int rank = m_gameSave->LenLeaderboard();
    for (int i = 0; i < m_gameSave->LenLeaderboard(); i++)
    {
        if (m_score > m_gameSave->GetScoreLeaderboard(i))
        {
            rank = i;
            break;
        }
    }

    if (rank == m_gameSave->LenLeaderboard())
    {
        m_gameSave->AddLeaderboard(m_score, _name);
    }
    else if ((rank < m_gameSave->LenLeaderboard()))
    {
        m_gameSave->InsertLeaderboard(m_score, _name, rank);
    }
    if (m_gameSave->LenLeaderboard() > 50)
    {
        m_gameSave->RemoveLeaderboard(rank);
    }
    SaveLeaderboard();
}

void UCustomGameInstance::GetLeaderboard(TArray<int32>& _score, TArray<FString>& _name)
{
    if (m_gameSave != nullptr)
    {
        _score = m_gameSave->GetScores();
        _name = m_gameSave->GetNames();
    }
}

void UCustomGameInstance::LoadLeaderboard()
{
    if (m_gameSave != nullptr &&  UGameplayStatics::DoesSaveGameExist("BombermanSave", 0))
    {
        m_gameSave = Cast<UCustomGameSave>(UGameplayStatics::LoadGameFromSlot("BombermanSave", 0));
    }
    else
    {
        m_gameSave = Cast<UCustomGameSave>(UGameplayStatics::CreateSaveGameObject(UCustomGameSave::StaticClass()));
        m_gameSave->DefaultLeaderboard();
        if (m_gameSave != nullptr)
        {
            SaveLeaderboard();
        }
    }
}

void UCustomGameInstance::SaveLeaderboard()
{
    UGameplayStatics::SaveGameToSlot(m_gameSave, TEXT("BombermanSave"), 0);
}

void UCustomGameInstance::PlayMusic(EGameState _state)
{
    switch (_state)
    {
    case EGameState::Menu:
        {
            UGameplayStatics::PlaySound2D(GetWorld(), m_menuMusic, 1, 1, 0);
            break;
        }
        default:
        {
            break;
        }
    }
}
