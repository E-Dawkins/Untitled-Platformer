// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelFinish.generated.h"

class UBoxComponent;
class UCircleFadeUserWidget;

UCLASS(HideCategories = (Rendering, Replication, Collision, HLOD, Physics, Networking, Input, Actor, LevelInstance, Cooking))
class PLATFORMER_API ALevelFinish : public AActor
{
	GENERATED_BODY()
	
public:
	ALevelFinish();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAnimFinish();

private:
	UPROPERTY(VisibleDefaultsOnly)
	UBoxComponent* TriggerCollider = nullptr;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> LevelToOpen = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCircleFadeUserWidget> LevelTransitionWidget;

	UPROPERTY()
	UCircleFadeUserWidget* LevelTransitionWidgetPtr = nullptr;

};
