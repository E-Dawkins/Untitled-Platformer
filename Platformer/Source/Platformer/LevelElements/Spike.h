// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spike.generated.h"

class UPaperSpriteComponent;
class UNiagaraSystem;

UCLASS(HideCategories = (Rendering, Replication, Collision, HLOD, Physics, Networking, Input, Actor, LevelInstance, Cooking))
class PLATFORMER_API ASpike : public AActor
{
	GENERATED_BODY()
	
public:
	ASpike();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleDefaultsOnly)
	UPaperSpriteComponent* Sprite = nullptr;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* PlayerDeathFX = nullptr;

	UPROPERTY(EditAnywhere)
	USoundWave* PlayerDeathSound = nullptr;

};
