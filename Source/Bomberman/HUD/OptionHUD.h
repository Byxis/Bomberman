#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/InputKeySelector.h"
#include "Components/Slider.h"
#include "OptionHUD.generated.h"

UCLASS()
class BOMBERMAN_API UOptionHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* m_buttonReturn = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UInputKeySelector* m_keyUp1 = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USlider* m_musicSlider = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USlider* m_jingleSlider = nullptr;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USlider* m_soundFxSlider = nullptr;

	UFUNCTION()
	void Quit();
	void SaveVolumes();
	void SaveKeys();

protected:
	virtual void NativeConstruct() override;
	
};
