// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelElements/Triggerable.h"
#include "Fan.generated.h"

class UPaperFlipbookComponent;
class UBoxComponent;
class UNiagaraComponent;
class UAudioComponent;

/**
 * 
 */
UCLASS()
class PLATFORMER_API AFan : public ATriggerable
{
	GENERATED_BODY()

	AFan();

protected:
	void BeginPlay() override;

public:
	void Tick(float DeltaSeconds) override;

private:
	void CheckForOverlappedActors();
	void AddForceToActor(FVector Force, AActor* Actor);

public:
	void OnActivate_Implementation() override;
	void OnDeactivate_Implementation() override;

private:
	UPROPERTY(VisibleDefaultsOnly)
	UPaperFlipbookComponent* Flipbook = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UBoxComponent* BlowArea = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UNiagaraComponent* FanFX = nullptr;

	UPROPERTY(EditAnywhere)
	USoundWave* FanSound = nullptr;

	UPROPERTY(EditAnywhere)
	float FanForce = 500.f;

	UPROPERTY()
	UAudioComponent* FanSoundComp = nullptr;
	
};
