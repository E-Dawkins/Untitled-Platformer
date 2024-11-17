// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "CircleFadeUserWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PLATFORMER_API UCircleFadeUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void PlayCircleShrink();

	UFUNCTION(BlueprintCallable)
	void PlayCircleGrow();

	UFUNCTION(BlueprintCallable)
	void SetCirclePosition(FVector2D ScreenPosition);

	UFUNCTION(BlueprintCallable)
	void SetCirclePositionWorld(FVector WorldPosition);

	UFUNCTION(BlueprintCallable)
	void BindToCircleAnimFinish(FWidgetAnimationDynamicEvent Event);

public:
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* CircleShrink = nullptr;

private:
	UPROPERTY(meta=(BindWidget))
	UImage* Image_Circle = nullptr;
	
};
