#include "PlayerHUD.h"
#include <string>
#include <Bomberman/Game/CustomGameInstance.h>
#include <Kismet/GameplayStatics.h>

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
        UCustomGameInstance* gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        if (gameInstance != nullptr)
        {
            SetScore(gameInstance->GetScore());
        }
        else
        {
            SetScore(0);
        }
    }
}
