// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/TriggerButton.h"

#include "PaperSpriteComponent.h"
#include "Triggerable.h"
#include "Components/BoxComponent.h"

ATriggerButton::ATriggerButton()
{
	PrimaryActorTick.bCanEverTick = true;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite Component");
	SpriteComponent->SetCollisionProfileName("OverlapAllDynamic");
	SetRootComponent(SpriteComponent);

	TriggerArea = CreateDefaultSubobject<UBoxComponent>("Trigger Area");
	TriggerArea->SetCollisionProfileName("OverlapAll");
	TriggerArea->SetupAttachment(SpriteComponent);
}

void ATriggerButton::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	bool CurrentButtonState = AreActorsOverlapping();

	if (CurrentButtonState != PreviousButtonState)
	{
		ToggleActorStates();
		UpdateSpriteState(CurrentButtonState);
	}

	PreviousButtonState = CurrentButtonState;
}

bool ATriggerButton::AreActorsOverlapping()
{
	TSet<AActor*> OverlappedActors = {};
	TriggerArea->GetOverlappingActors(OverlappedActors);

	for (AActor* Actor : OverlappedActors)
	{
		if (Actor == this)
			continue;

		return true;
	}

	return false;
}

void ATriggerButton::ToggleActorStates()
{
	for (ATriggerable* Triggerable : ActorsToTrigger)
	{
		if (IsValid(Triggerable))
		{
			Triggerable->ToggleActivationState();
		}
	}
}

void ATriggerButton::UpdateSpriteState(bool bState)
{
	SpriteComponent->SetSprite(bState ? Sprite_Activated : Sprite_Deactivated);
}
