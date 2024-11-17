// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BouncyPlatform.generated.h"

class UBoxComponent;

UCLASS(HideCategories = (Rendering, Replication, Collision, HLOD, Physics, Networking, Input, Actor, LevelInstance, Cooking))
class PLATFORMER_API ABouncyPlatform : public AActor
{
	GENERATED_BODY()
	
public:
	ABouncyPlatform();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* Collider = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* Trigger = nullptr;

	UPROPERTY(EditAnywhere)
	FVector BounceNormal = FVector::ZAxisVector;

	UPROPERTY()
	TMap<AActor*, float> ActorVelocities = {};

};
