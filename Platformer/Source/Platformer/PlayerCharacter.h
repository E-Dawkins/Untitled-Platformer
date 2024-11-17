// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UPaperSpriteComponent;
class UCircleFadeUserWidget;
class UPostProcessComponent;

/**
 * 
 */
UCLASS()
class PLATFORMER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnDeath();

private:
	void MoveRight(float Value);

	void TryJump();
	void CalculateJumpVelocity() const;

	void OnInput_Restart();
	void OnInput_Exit();

	UFUNCTION()
	void SetupTransitionWidget(bool bGrow);

	UFUNCTION()
	void RestartLevel();

	UFUNCTION()
	void ExitToMainMenu();

private:
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* PlayerSprite = nullptr;

	UPROPERTY(EditAnywhere, Category="Player Character") // in cm
	float JumpHeight = 100.f;

	UPROPERTY(EditAnywhere, Category = "Player Character")
	TSoftObjectPtr<UWorld> MainMenuLevel = nullptr;

	UPROPERTY(EditAnywhere, Category = "Player Character")
	TSubclassOf<UCircleFadeUserWidget> TransitionWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Player Character", meta = (ClampMin = "0")) // in seconds
	float JumpBuffer = 0.15f;

	float CurrentJumpBuffer = 0.f;

	UPROPERTY(EditAnywhere, Category = "Player Character", meta = (ClampMin = "0")) // in seconds
	float CoyoteTime = 0.1f;

	float CurrentCoyoteTime = 0.f;

	UPROPERTY()
	UCircleFadeUserWidget* TransitionWidgetPtr = nullptr;

	FTimerHandle TransitionHandle = {};
};
