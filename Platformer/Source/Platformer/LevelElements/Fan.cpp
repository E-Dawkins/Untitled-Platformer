// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/Fan.h"

#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

AFan::AFan()
{
	PrimaryActorTick.bCanEverTick = true;

	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>("Flipbook");
	Flipbook->SetCollisionProfileName("OverlapAll");
	SetRootComponent(Flipbook);

	BlowArea = CreateDefaultSubobject<UBoxComponent>("BlowArea");
	BlowArea->SetCollisionProfileName("OverlapAll");
	BlowArea->SetupAttachment(Flipbook);

	FanFX = CreateDefaultSubobject<UNiagaraComponent>("Fan FX");
	FanFX->SetupAttachment(Flipbook);
}

void AFan::BeginPlay()
{
	Super::BeginPlay();

	Flipbook->SetPlaybackPositionInFrames(0, false);
}

void AFan::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bActivated)
		CheckForOverlappedActors();
}

void AFan::CheckForOverlappedActors()
{
	FVector Force = GetActorUpVector() * FanForce;

	TSet<AActor*> OverlappedActors = {};
	BlowArea->GetOverlappingActors(OverlappedActors);

	for (AActor* Actor : OverlappedActors)
	{
		if (Actor == this)
			continue;

		AddForceToActor(Force, Actor);
	}
}

void AFan::AddForceToActor(FVector Force, AActor* Actor)
{
	float DistanceAlongFanNormal = FVector::PointPlaneDist(Actor->GetActorLocation(), Flipbook->GetComponentLocation(), GetActorUpVector());
	float PercentageFromEnd = DistanceAlongFanNormal / (BlowArea->GetScaledBoxExtent().Z * 2.f);

	Force *= FMath::Clamp(1.f - PercentageFromEnd, 0.f, 1.f);

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(Actor))
	{
		UCharacterMovementComponent* MovementComp = Player->GetCharacterMovement();
		MovementComp->AddForce(Force * MovementComp->Mass * MovementComp->GravityScale);
	}
	else if (UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent()))
	{
		if (PrimitiveComp->IsSimulatingPhysics())
		{
			PrimitiveComp->AddForce(Force * PrimitiveComp->GetMass());
		}
	}
}

void AFan::OnActivate_Implementation()
{
	Flipbook->Play();
	FanFX->Activate();

	FanSoundComp = UGameplayStatics::CreateSound2D(this, FanSound);
	if (IsValid(FanSoundComp)) FanSoundComp->Activate();
}

void AFan::OnDeactivate_Implementation()
{
	Flipbook->Stop();
	FanFX->Deactivate();

	if (IsValid(FanSoundComp)) FanSoundComp->Deactivate();
}
