// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Triggerable.h"
#include "Sawblade.generated.h"

class UPaperFlipbookComponent;
class USplineComponent;
class UNiagaraSystem;

UENUM()
enum ESawPathType
{
	ESPT_Mod UMETA(DisplayName = "Mod", ToolTip = "(Time % Duration)"),
	ESPT_Once UMETA(DisplayName = "Once", ToolTip = "Clamp(Time, Duration)"),
	ESPT_PingPong UMETA(DisplayName = "Ping Pong", ToolTip = "Time = (0 <-> Duration)")
};

UCLASS()
class PLATFORMER_API ASawblade : public ATriggerable
{
	GENERATED_BODY()
	
public:
	ASawblade();

protected:
	void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float GetModifiedCurrentTime(float DeltaTime);

private:
	UPROPERTY(VisibleDefaultsOnly)
	UPaperFlipbookComponent* Sprite = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	USplineComponent* SawbladePath = nullptr;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* PlayerDeathFX = nullptr;

	UPROPERTY(EditAnywhere)
	USoundWave* PlayerDeathSound = nullptr;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* BoxShredFX = nullptr;

	UPROPERTY(EditAnywhere)
	USoundWave* BoxShredSound = nullptr;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ESawPathType> SawPathType = ESPT_Mod;

	UPROPERTY(VisibleAnywhere)
	float CurrentTime = 0.f;

	UPROPERTY(VisibleAnywhere)
	bool bDirection = true;

};
