#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "StartingHUD.generated.h"

UCLASS()
class BOMBERMAN_API UStartingHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetLevelText();
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_levelText = nullptr;


	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_limitText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_powerText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_speedText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_detonatorText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_vestText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_ghostWallsText = nullptr;
};
