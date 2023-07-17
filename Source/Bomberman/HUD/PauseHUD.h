#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "PauseHUD.generated.h"

UCLASS()
class BOMBERMAN_API UPauseHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetScore(int32 _score);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* m_scoreText = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* m_buttonPlay = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* m_buttonSettings = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* m_buttonQuit = nullptr;

	UFUNCTION()
	void UnPauseGame();
	UFUNCTION()
	void Quit();

protected:
	virtual void NativeConstruct() override;
};
