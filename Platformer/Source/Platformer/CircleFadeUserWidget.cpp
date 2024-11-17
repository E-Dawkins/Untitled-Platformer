// Fill out your copyright notice in the Description page of Project Settings.


#include "CircleFadeUserWidget.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Camera/CameraActor.h"
#include "Camera/PlayerCameraManager.h"

void UCircleFadeUserWidget::PlayCircleShrink()
{
	if (CircleShrink)
		PlayAnimationForward(CircleShrink);
}

void UCircleFadeUserWidget::PlayCircleGrow()
{
	if (CircleShrink)
		PlayAnimationReverse(CircleShrink);
}

void UCircleFadeUserWidget::SetCirclePosition(FVector2D ScreenPosition)
{
	if (Image_Circle)
	{
		UMaterialInstanceDynamic* Material = UWidgetBlueprintLibrary::GetDynamicMaterial(Image_Circle->Brush);
		Material->SetVectorParameterValue("CircleCenter", FVector(ScreenPosition.X, ScreenPosition.Y, 0));
	}
}

void UCircleFadeUserWidget::SetCirclePositionWorld(FVector WorldPosition)
{
	FVector2D ScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), WorldPosition, ScreenPosition);

	SetCirclePosition(ScreenPosition);
}

void UCircleFadeUserWidget::BindToCircleAnimFinish(FWidgetAnimationDynamicEvent Event)
{
	BindToAnimationFinished(CircleShrink, Event);
}
