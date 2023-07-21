#include "MainmenuHUD.h"

#include <Bomberman/Game/CustomGameInstance.h>
#include "Kismet/GameplayStatics.h"
#include <string>
#include <Bomberman/PlayerManager/PlayerControl.h>

void UMainmenuHUD::NativeConstruct()
{
    Super::NativeConstruct();

    if (m_buttonPlay != nullptr)
        m_buttonPlay->OnClicked.AddDynamic(this, &UMainmenuHUD::StartPlaying);

    if (m_buttonOption != nullptr)
        m_buttonOption->OnClicked.AddDynamic(this, &UMainmenuHUD::OpenOptionMenu);

    if (m_buttonQuit != nullptr)
        m_buttonQuit->OnClicked.AddDynamic(this, &UMainmenuHUD::Quit);

    m_scores.Add(m_score1);
    m_scores.Add(m_score2);
    m_scores.Add(m_score3);
    m_scores.Add(m_score4);
    m_scores.Add(m_score5);
    m_scores.Add(m_score6);
    m_scores.Add(m_score7);
    m_scores.Add(m_score8);
    m_scores.Add(m_score9);

    SetLeaderboard();
}

void UMainmenuHUD::SetLeaderboard()
{
    UCustomGameInstance* gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    TArray<int32> scoreArray;
    TArray<FString> nameArray;
    gameInstance->GetLeaderboard(scoreArray, nameArray);
    for (int i = 0; i < 9; i++)
    {
        if (i < scoreArray.Num())
        {
            std::string var = std::to_string(i+1) + " - " + TCHAR_TO_UTF8(*nameArray[i]) + "     " + std::to_string(scoreArray[i]);
            m_scores[i]->SetText(FText::FromString(var.c_str()));
        }
        else
        {
            m_scores[i]->SetText(FText::FromString(" "));
        }
    }
}

void UMainmenuHUD::StartPlaying()
{
    UCustomGameInstance* gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (gameInstance != nullptr)
    {
        gameInstance->ResetAll();
        UGameplayStatics::OpenLevel(GetWorld(), "Level_1");
    }
}

void UMainmenuHUD::OpenOptionMenu()
{
    APlayerControl* playerControl = Cast<APlayerControl>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (playerControl != nullptr)
    {
        playerControl->OpenOptionsMenu();
    }
}

void UMainmenuHUD::Quit()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
