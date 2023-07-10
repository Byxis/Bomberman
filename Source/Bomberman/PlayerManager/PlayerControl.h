#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/Capsulecomponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementcomponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraActor.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.h"
#include "PlayerControl.generated.h"

UCLASS()
class BOMBERMAN_API APlayerControl : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerControl();
	virtual ~APlayerControl() = default;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* SpringArmComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* CameraComp = nullptr;

	void MoveForward(float _axis);
	void MoveRight(float _axis);
	void SpawnBomb();
	void GiveBackBomb();
	void SetCanPlaceBomb(bool _bool);
	void Damage();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void BonusBombPower();
	void BonusBombLimit();
	void ActualiseScore(int32 _score);
	void PauseGame();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_bomb = nullptr;

	int32 m_maxPlacedBomb = 1;
	bool m_dead = false;
	bool m_canMove = true;
	int32 m_power = 1;
	bool m_canPlaceBomb = true;
	UPROPERTY(Editanywhere)
		TSubclassOf<UPlayerHUD> m_playerHudClass = nullptr;
	UPROPERTY()
		class UPlayerHUD* m_playerHud = nullptr;
};
