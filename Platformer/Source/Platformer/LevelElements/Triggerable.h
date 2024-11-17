// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Triggerable.generated.h"

/**
 * 
 */
UCLASS(HideCategories = (Rendering, Replication, Collision, HLOD, Physics, Networking, Input, Actor, LevelInstance, Cooking))
class PLATFORMER_API ATriggerable : public AActor
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnActivate();

	virtual void OnActivate_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void OnDeactivate();

	virtual void OnDeactivate_Implementation() {}

	virtual void BeginPlay() override
	{
		Super::BeginPlay();

		if (bActivated) OnActivate();
		else OnDeactivate();
	}

public:
	void ToggleActivationState()
	{
		bActivated = !bActivated;

		if (bActivated) OnActivate();
		else OnDeactivate();
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Triggerable")
	bool bActivated = false;
	
};
