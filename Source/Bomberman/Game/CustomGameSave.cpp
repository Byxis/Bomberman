#include "CustomGameSave.h"


UCustomGameSave::UCustomGameSave()
{}

void UCustomGameSave::Default()
{
    DefaultLeaderboard();
    m_musicVolume = 1;
    m_jingleVolume = 1;
    m_sfxVolume = 1;
}

void UCustomGameSave::DefaultLeaderboard()
{
    m_scoreArray.Add(500);
    m_nameArray.Add(FString("Alexis"));
}

void UCustomGameSave::AddLeaderboard(int32 _score, FString _name)
{
    m_scoreArray.Add(_score);
    m_nameArray.Add(_name);
}

int32 UCustomGameSave::GetScoreLeaderboard(int _i)
{
    if (_i >= m_scoreArray.Num())
        return -1;
    return m_scoreArray[_i];
}

int UCustomGameSave::LenLeaderboard()
{
    return m_scoreArray.Num();
}

void UCustomGameSave::InsertLeaderboard(int32 _score, FString _name, int _i)
{
    m_scoreArray.Insert(_score, _i);
    m_nameArray.Insert(_name, _i);
}

void UCustomGameSave::RemoveLeaderboard(int _i)
{
    m_scoreArray.RemoveAt(_i);
    m_nameArray.RemoveAt(_i);
}

TArray<int32> UCustomGameSave::GetScores()
{
    return m_scoreArray;
}

TArray<FString> UCustomGameSave::GetNames()
{
    return m_nameArray;
}

float UCustomGameSave::GetMusicVolume()
{
    return m_musicVolume;
}

void UCustomGameSave::SetMusicVolume(float _volume)
{
    m_musicVolume = _volume;
}

float UCustomGameSave::GetSFXVolume()
{
    return m_sfxVolume;
}

void UCustomGameSave::SetSFXVolume(float _volume)
{
    m_sfxVolume = _volume;
}

float UCustomGameSave::GetJingleVolume()
{
    return m_jingleVolume;
}

void UCustomGameSave::SetJingleVolume(float _volume)
{
    m_jingleVolume = _volume;
}
