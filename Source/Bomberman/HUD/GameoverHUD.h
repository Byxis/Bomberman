#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "GameoverHUD.generated.h"

UCLASS()
class BOMBERMAN_API UGameoverHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetScore(int32 _score);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* m_scoreText = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* m_saveText = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* m_name = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* m_buttonQuit = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* m_buttonSave = nullptr;

	UFUNCTION()
	void SaveScore();
	UFUNCTION()
	void Quit();

protected:
	virtual void NativeConstruct() override;
	bool m_isSaved = false;
};
