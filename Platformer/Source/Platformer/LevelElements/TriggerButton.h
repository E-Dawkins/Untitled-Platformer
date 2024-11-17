// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerButton.generated.h"

class UPaperSpriteComponent;
class UPaperSprite;
class ATriggerable;
class UBoxComponent;

UCLASS(HideCategories = (Rendering, Replication, Collision, HLOD, Physics, Networking, Input, Actor, LevelInstance, Cooking))
class PLATFORMER_API ATriggerButton : public AActor
{
	GENERATED_BODY()
	
public:
	ATriggerButton();

	void Tick(float DeltaSeconds) override;

private:
	bool AreActorsOverlapping();

	void ToggleActorStates();
	void UpdateSpriteState(bool bState);

private:
	UPROPERTY(VisibleDefaultsOnly)
	UPaperSpriteComponent* SpriteComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UBoxComponent* TriggerArea = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UPaperSprite* Sprite_Deactivated = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UPaperSprite* Sprite_Activated = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<ATriggerable*> ActorsToTrigger;

	bool PreviousButtonState = false;

};
