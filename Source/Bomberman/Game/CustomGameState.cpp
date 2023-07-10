// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameState.h"
#include "CustomGameMode.h"

ACustomGameState::ACustomGameState()
{
	m_level = 0;
}

void ACustomGameState::SetLevel(int32 _newLevel)
{
	m_level = _newLevel;
}

int32 ACustomGameState::GetLevel()
{
	return m_level;
}

void ACustomGameState::AddScore(int32 _score)
{
	m_score += _score;
}

int32 ACustomGameState::GetScore()
{
	return m_score;
}
