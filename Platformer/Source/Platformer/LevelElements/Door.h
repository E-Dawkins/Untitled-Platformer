// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelElements/Triggerable.h"
#include "Door.generated.h"

class UPaperFlipbookComponent;

/**
 * 
 */
UCLASS()
class PLATFORMER_API ADoor : public ATriggerable
{
	GENERATED_BODY()

	ADoor();

protected:
	void BeginPlay() override;

public:
	void OnActivate_Implementation() override;
	void OnDeactivate_Implementation() override;

private:
	UPROPERTY(VisibleDefaultsOnly)
	UPaperFlipbookComponent* Flipbook = nullptr;

	UPROPERTY(EditAnywhere)
	int FlipbookStartFrame = 0;

	UPROPERTY(EditAnywhere)
	USoundWave* DoorOpenSound = nullptr;

	UPROPERTY(EditAnywhere)
	USoundWave* DoorCloseSound = nullptr;
	
};
