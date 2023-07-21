#include "StartingHUD.h"
#include "Kismet/GameplayStatics.h"

void UStartingHUD::NativeConstruct()
{
    Super::NativeConstruct();

    if (m_levelText != nullptr)
    {
        SetLevelText();
    }
}

void UStartingHUD::SetLevelText()
{
    FString levelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
    levelName = levelName.Replace(TEXT("_"), TEXT(" "));
    m_levelText->SetText(FText::FromString(levelName));
}
