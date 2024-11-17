// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlipperyPlatform.generated.h"

class UBoxComponent;
class APlayerCharacter;

UCLASS(HideCategories = (Rendering, Replication, Collision, HLOD, Physics, Networking, Input, Actor, LevelInstance, Cooking))
class PLATFORMER_API ASlipperyPlatform : public AActor
{
	GENERATED_BODY()
	
public:
	ASlipperyPlatform();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* Collider = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* Trigger = nullptr;

	UPROPERTY(EditAnywhere)
	float PlayerGroundFriction_Normal = 8.f;

	UPROPERTY(EditAnywhere)
	float PlayerDeceleration_Normal = 2048.f;

	UPROPERTY(EditAnywhere)
	float PlayerGroundFriction_Slippery = 0.1f;

	UPROPERTY(EditAnywhere)
	float PlayerDeceleration_Slippery = 512.f;

};
