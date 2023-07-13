#include "GameoverHUD.h"

#include <string>
#include "./Bomberman/PlayerManager/PlayerControl.h"
#include <Bomberman/Game/CustomGameInstance.h>
#include "Kismet/GameplayStatics.h"

void UGameoverHUD::NativeConstruct()
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
    if (m_saveText != nullptr)
    {
        m_saveText->SetText(FText::FromString(""));
    }
    if (m_buttonSave != nullptr)
    {
        m_buttonSave->OnClicked.AddDynamic(this, &UGameoverHUD::SaveScore);
    }
    if (m_buttonQuit != nullptr)
    {
        m_buttonQuit->OnClicked.AddDynamic(this, &UGameoverHUD::Quit);
    }
}

void UGameoverHUD::SetScore(int32 _score)
{
    if (m_scoreText != nullptr)
    {
        std::string var = "Score : " + std::to_string(_score);
        m_scoreText->SetText(FText::FromString(var.c_str()));
    }
}

void UGameoverHUD::SaveScore()
{
    if (m_saveText != nullptr && !m_isSaved)
    {
        //TODO: save score
        m_saveText->SetText(FText::FromString("Score saved !"));
        m_isSaved = true;
    }
}

void UGameoverHUD::Quit()
{

}
