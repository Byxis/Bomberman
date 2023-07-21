#include "OptionHUD.h"
#include <Bomberman/PlayerManager/PlayerControl.h>
#include <GameFramework/InputSettings.h>

void UOptionHUD::Quit()
{
    SaveKeys();
    SaveVolumes();
    APlayerControl* playerControl = Cast<APlayerControl>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (playerControl != nullptr)
    {
        playerControl->CloseOptionsMenu();
    }
}

void UOptionHUD::SaveVolumes()
{
    UCustomGameInstance* gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    gameInstance->SetMusicVolume(m_musicSlider->GetValue());
}

void UOptionHUD::SaveKeys()
{
}

void UOptionHUD::NativeConstruct()
{
    if (m_buttonReturn != nullptr)
    {
        m_buttonReturn->OnClicked.AddDynamic(this, &UOptionHUD::Quit);
    }
    if (m_musicSlider != nullptr)
    {
        UCustomGameInstance* gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        if (gameInstance != nullptr)
        {
            m_musicSlider->SetValue(gameInstance->GetMusicVolume());
        }
    }
    if (m_soundFxSlider != nullptr)
    {
        UCustomGameInstance* gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        if (gameInstance != nullptr)
        {
            m_soundFxSlider->SetValue(gameInstance->GetMusicVolume());
        }
    }
}
