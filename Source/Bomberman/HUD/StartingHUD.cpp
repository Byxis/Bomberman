#include "StartingHUD.h"
#include "Kismet/GameplayStatics.h"
#include <Bomberman/Game/CustomGameInstance.h>
#include <string>

void UStartingHUD::NativeConstruct()
{
    Super::NativeConstruct();

    if (m_levelText != nullptr)
    {
        SetLevelText();
    }

    UCustomGameInstance* gameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (gameInstance == nullptr)
        return;

    if (m_limitText != nullptr)
    {
        std::string var = std::to_string(gameInstance->GetBombLimit()) + "/" + std::to_string(gameInstance->GetMaxBombLimit());
        m_limitText->SetText(FText::FromString(var.c_str()));
    }
    if(m_powerText != nullptr)
    {
        std::string var = std::to_string(gameInstance->GetBombPower()) + "/" + std::to_string(gameInstance->GetMaxBombPower());
        m_powerText->SetText(FText::FromString(var.c_str()));
    }
    if(m_speedText != nullptr)
    {
        int speedLevel = 1 + ((int)((gameInstance->GetSpeed() - 0.9f) / 0.5f));
        int maxSpeedLevel = 1 + ((int)((gameInstance->GetMaxSpeed() - 0.9f) / 0.5f));
        std::string var = std::to_string(speedLevel) + "/" + std::to_string(maxSpeedLevel);
        m_speedText->SetText(FText::FromString(var.c_str()));
    }
    if(m_detonatorText != nullptr && !gameInstance->HasDetonatorBonus())
    {
        m_detonatorText->SetText(FText::FromString("X"));
    }
    if(m_vestText != nullptr && !gameInstance->HasVestBonus())
    {
        m_vestText->SetText(FText::FromString("X"));
    }
    if(m_ghostWallsText != nullptr && !gameInstance->HasGhostWallsBonus())
    {
        m_ghostWallsText->SetText(FText::FromString("X"));
    }
}

void UStartingHUD::SetLevelText()
{
    FString levelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
    levelName = levelName.Replace(TEXT("_"), TEXT(" "));
    m_levelText->SetText(FText::FromString(levelName));
}
