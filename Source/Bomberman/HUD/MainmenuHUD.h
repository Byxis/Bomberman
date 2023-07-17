#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MainmenuHUD.generated.h"

UCLASS()
class BOMBERMAN_API UMainmenuHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* m_buttonPlay = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* m_buttonSettings = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* m_buttonQuit = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* m_score1 = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_score2 = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_score3 = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_score4 = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_score5 = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_score6 = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_score7 = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_score8 = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* m_score9 = nullptr;

	UFUNCTION()
	void StartPlaying();
	UFUNCTION()
	void Quit();

protected:
	virtual void NativeConstruct() override;

private:
	TArray<UTextBlock*> m_scores;
	void SetLeaderboard();
};
