// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/LevelFinish.h"

#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "CircleFadeUserWidget.h"

ALevelFinish::ALevelFinish()
{
	TriggerCollider = CreateDefaultSubobject<UBoxComponent>("Trigger Collider");
	SetRootComponent(TriggerCollider);
}

void ALevelFinish::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerCollider->OnComponentBeginOverlap.AddDynamic(this, &ALevelFinish::OnComponentBeginOverlap);

	LevelTransitionWidgetPtr = CreateWidget<UCircleFadeUserWidget>(GetWorld()->GetFirstPlayerController(), LevelTransitionWidget);
	if (LevelTransitionWidgetPtr)
	{
		LevelTransitionWidgetPtr->AddToViewport();
	}
}

void ALevelFinish::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (LevelTransitionWidgetPtr)
		{
			FVector2D ScreenPosition;
			UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), Player->GetActorLocation(), ScreenPosition);
			LevelTransitionWidgetPtr->SetCirclePosition(ScreenPosition);
			LevelTransitionWidgetPtr->PlayCircleShrink();

			FWidgetAnimationDynamicEvent Event;
			Event.BindDynamic(this, &ALevelFinish::OnAnimFinish);
			LevelTransitionWidgetPtr->BindToCircleAnimFinish(Event);

			APlayerController* PController = GetWorld()->GetFirstPlayerController();
			PController->DisableInput(PController);
		}
	}
}

void ALevelFinish::OnAnimFinish()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LevelToOpen);
}
