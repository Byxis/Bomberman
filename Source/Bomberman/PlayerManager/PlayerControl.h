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
#include "./Bomberman/HUD/PauseHUD.h"
#include "./Bomberman/HUD/GameoverHUD.h"
#include "./Bomberman/HUD/MainmenuHUD.h"
#include "./Bomberman/HUD/StartingHUD.h"
#include <Bomberman/Game/CustomGameInstance.h>
#include <Bomberman/Game/CustomGameMode.h>	
#include "Components/AudioComponent.h"
#include <Bomberman/HUD/OptionHUD.h>
#include "LevelSequenceActor.h"
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
	void Detonatebomb();
	void GiveBackBomb();
	void SetCanPlaceBomb(bool _bool);
	void Damage();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void BonusBombPower();
	void BonusBombLimit();
	void BonusSpeed();
	void BonusDetonator();
	void BonusVest();
	void BonusGhostWalls();

	void ActualiseScore();
	void ActualiseTimer();
	void ActualiseLife();
	void PauseGame();
	void OpenMainMenu();
	void OpenOptionsMenu();
	void CloseOptionsMenu();
	bool IsDead();
	bool IsPlacingBomb();
	void SetPlacingBomb(bool _bool);
	bool hasPlaceToSpawnBomb();
	UFUNCTION()
	void OnInteract(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void PlayMusic();
	void PauseMusic(bool _bool);
	void SetMusicVolume(float _volume);
	void PlayJingle();
	void PauseJingle(bool _bool);
	void SetJingleVolume(float _volume);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_bomb = nullptr;

	UCustomGameInstance* m_gameInstance = nullptr;
	ACustomGameMode* m_gameMode = nullptr;
	int32 m_maxPlacedBomb = 1;
	bool m_dead = false;
	bool m_isRespawning = false;
	float m_respawningTimer = 8.0f;
	bool m_canPlaceBomb = true;
	bool m_isPlacingBomb = false;
	float m_startDelay = 4.2f;
	bool m_hasEnded = false;
	bool m_isLoaded = false;

	UPROPERTY(Editanywhere)
	TSubclassOf<UPlayerHUD> m_playerHudClass = nullptr;
	UPROPERTY()
	class UPlayerHUD* m_playerHud = nullptr;
	UPROPERTY(Editanywhere)
	TSubclassOf<UPauseHUD> m_pauseHudClass = nullptr;
	UPROPERTY()
	class UPauseHUD* m_pauseHud = nullptr;
	UPROPERTY(Editanywhere)
	TSubclassOf<UGameoverHUD> m_gameOverHudClass = nullptr;
	UPROPERTY()
	class UGameoverHUD* m_gameOverHud = nullptr;
	UPROPERTY(Editanywhere)
	TSubclassOf<UMainmenuHUD> m_mainMenuHudClass = nullptr;
	UPROPERTY()
	class UMainmenuHUD* m_mainMenuHud = nullptr;
	UPROPERTY(Editanywhere)
	TSubclassOf<UStartingHUD> m_startingHudClass = nullptr;
	UPROPERTY()
	class UStartingHUD* m_startingHud = nullptr;
	UPROPERTY(Editanywhere)
	TSubclassOf<UOptionHUD> m_optionHudClass = nullptr;
	UPROPERTY()
	class UOptionHUD* m_optionHud = nullptr;
	UPROPERTY(Editanywhere)
	TSubclassOf<UGameoverHUD> m_endHudClass = nullptr;
	UPROPERTY()
	class UGameoverHUD* m_endHud = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	ULevelSequence* m_endSequence = nullptr;

	//Musics
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USoundCue* m_menuMusic = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USoundCue* m_levelMusic = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USoundCue* m_bonusLevelMusic = nullptr;

	//Jingle
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USoundCue* m_startingJingle = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USoundCue* m_deathJingle = nullptr;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USoundCue* m_levelEndingJingle = nullptr;

	UAudioComponent* m_currentMusic = nullptr;
	UAudioComponent* m_currentJingle = nullptr;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UTouchInterface* m_touchInterface = nullptr;

	void LoadPlayerControl();
};
