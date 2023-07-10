#include "PlayerHUD.h"
#include <string>

void UPlayerHUD::SetScore(int32 _score)
{
    if (m_scoreText != nullptr)
    {
        std::string var = "Score : " + std::to_string(_score);
        m_scoreText->SetText(FText::FromString(var.c_str()));
    }
}

void UPlayerHUD::NativeConstruct()
{
    Super::NativeConstruct();

    if (m_scoreText != nullptr)
    {
        SetScore(0);
    }
}
