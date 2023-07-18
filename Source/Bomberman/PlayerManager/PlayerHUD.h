#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PlayerHUD.generated.h"

UCLASS()
class BOMBERMAN_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetScore(int32 _score);
	void SetTime(int32 _time);
	void SetLife(int32 _life);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* m_scoreText = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* m_timeText = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* m_lifeText = nullptr;

protected:
	virtual void NativeConstruct() override;
};
