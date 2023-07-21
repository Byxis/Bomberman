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

bool UCustomGameInstance::HasVestBonus()
{
    return m_vest;
}

void UCustomGameInstance::SetVestBonus(bool _bool)
{
    m_vest = _bool;
}

bool UCustomGameInstance::HasGhostWallsBonus()
{
    return m_ghostWalls;
}

void UCustomGameInstance::SetGhostWallsBonus(bool _bool)
{
    m_ghostWalls = _bool;
}

void UCustomGameInstance::ResetAll()
{
    m_bombLimit = 1;
    m_maxBombLimit = 8;
    m_bombPower = 1;
    m_maxBombPower = 8;
    m_score = 0;
    m_life = 2;
    m_speed = 0.9;
    m_maxSpeed = 2.4;
    m_detonator = false;
    m_vest = false;
    m_ghostWalls = false;
}

void UCustomGameInstance::AddToLeaderboard(FString _name)
{
    UCustomGameSave* gameSave = GetActualGameSave();

    int rank = gameSave->LenLeaderboard();
    for (int i = 0; i < gameSave->LenLeaderboard(); i++)
    {
        if (m_score > gameSave->GetScoreLeaderboard(i))
        {
            rank = i;
            break;
        }
    }

    if (rank == gameSave->LenLeaderboard())
    {
        gameSave->AddLeaderboard(m_score, _name);
    }
    else if ((rank < gameSave->LenLeaderboard()))
    {
        gameSave->InsertLeaderboard(m_score, _name, rank);
    }
    if (gameSave->LenLeaderboard() > 50)
    {
        gameSave->RemoveLeaderboard(rank);
    }
    SaveGameSave(gameSave);
}

void UCustomGameInstance::GetLeaderboard(TArray<int32>& _score, TArray<FString>& _name)
{
    UCustomGameSave* gameSave = GetActualGameSave();
    if (gameSave != nullptr)
    {
        _score = gameSave->GetScores();
        _name = gameSave->GetNames();
    }
}

UCustomGameSave* UCustomGameInstance::GetActualGameSave()
{
    UCustomGameSave* gameSave = nullptr;

    if (UGameplayStatics::DoesSaveGameExist("BombermanSave", 0))
    {
        gameSave = Cast<UCustomGameSave>(UGameplayStatics::LoadGameFromSlot("BombermanSave", 0));
    }
    else
    {
        gameSave = Cast<UCustomGameSave>(UGameplayStatics::CreateSaveGameObject(UCustomGameSave::StaticClass()));
        gameSave->Default();
        if (gameSave != nullptr)
        {
            SaveGameSave(gameSave);
        }
    }
    return gameSave;
}

void UCustomGameInstance::SaveGameSave(UCustomGameSave* _gameSave)
{
    UE_LOG(LogTemp, Warning, TEXT("Saved"))
    UGameplayStatics::SaveGameToSlot(_gameSave, TEXT("BombermanSave"), 0);
}

int UCustomGameInstance::GetLife()
{
    return m_life;
}

void UCustomGameInstance::AddLife(int _amount)
{
    m_life += _amount;
}

float UCustomGameInstance::GetMusicVolume()
{
    if (m_musicVolume == -1)
    {
        UCustomGameSave* gameSave = GetActualGameSave();
        if (gameSave != nullptr)
        {
            m_musicVolume = gameSave->GetMusicVolume();
        }
    }
    return m_musicVolume;
}

void UCustomGameInstance::SetMusicVolume(float _amount)
{
    m_musicVolume = _amount;
    UCustomGameSave* gameSave = GetActualGameSave();
    gameSave->SetMusicVolume(m_musicVolume);
    SaveGameSave(gameSave);
    APlayerControl* playerControl = Cast<APlayerControl>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (playerControl != nullptr)
    {
        playerControl->SetMusicVolume(_amount);
    }
}

float UCustomGameInstance::GetSFXVolume()
{
    if (m_sfxVolume == -1)
    {
        UCustomGameSave* gameSave = GetActualGameSave();
        if (gameSave != nullptr)
        {
            m_sfxVolume = gameSave->GetSFXVolume();
        }
    }
    return m_sfxVolume;
}

void UCustomGameInstance::SetSFXVolume(float _amount)
{
    m_sfxVolume = _amount;
    UCustomGameSave* gameSave = GetActualGameSave();
    gameSave->SetSFXVolume(m_sfxVolume);
    SaveGameSave(gameSave);

}

float UCustomGameInstance::GetJingleVolume()
{
    return 0.0f;
}
