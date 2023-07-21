#include "PauseHUD.h"

#include <string>
#include "./Bomberman/PlayerManager/PlayerControl.h"
#include <Bomberman/Game/CustomGameInstance.h>
#include "Kismet/GameplayStatics.h"

void UPauseHUD::SetScore(int32 _score)
{
    if (m_scoreText != nullptr)
    {
        std::string var = "Score : " + std::to_string(_score);
        m_scoreText->SetText(FText::FromString(var.c_str()));
    }
}

void UPauseHUD::NativeConstruct()
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
    if (m_buttonPlay != nullptr)
        m_buttonPlay->OnClicked.AddDynamic(this, &UPauseHUD::UnPauseGame);

    if (m_buttonOptions != nullptr)
        m_buttonOptions->OnClicked.AddDynamic(this, &UPauseHUD::OpenOptions);

    if (m_buttonQuit != nullptr)
        m_buttonQuit->OnClicked.AddDynamic(this, &UPauseHUD::Quit);
}

void UPauseHUD::UnPauseGame()
{
    APlayerControl* playerControl = Cast<APlayerControl>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (playerControl != nullptr)
    {
        playerControl->PauseGame();
    }
}

void UPauseHUD::OpenOptions()
{
    APlayerControl* playerControl = Cast<APlayerControl>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (playerControl != nullptr)
    {
        playerControl->OpenOptionsMenu();
    }
}

void UPauseHUD::Quit()
{
    UGameplayStatics::OpenLevel(GetWorld(), "Level_0");
}
