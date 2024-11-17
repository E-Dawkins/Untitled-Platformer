// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/Door.h"

#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"

ADoor::ADoor()
{
	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>("Flipbook");
	Flipbook->SetCollisionProfileName("BlockAll");
	Flipbook->SetLooping(false);
	SetRootComponent(Flipbook);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	Flipbook->SetPlaybackPositionInFrames(FlipbookStartFrame, false);
}

void ADoor::OnActivate_Implementation()
{
	Flipbook->Play();
	SetActorEnableCollision(false);

	UGameplayStatics::PlaySound2D(this, DoorOpenSound);
}

void ADoor::OnDeactivate_Implementation()
{
	Flipbook->Reverse();
	SetActorEnableCollision(true);

	UGameplayStatics::PlaySound2D(this, DoorCloseSound);
}
