// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/Sawblade.h"

#include "PaperFlipbookComponent.h"
#include "Components/SplineComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASawblade::ASawblade()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* DefaultRoot = CreateDefaultSubobject<USceneComponent>("Default Root");

	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>("Sprite");
	Sprite->SetupAttachment(DefaultRoot);

	SawbladePath = CreateDefaultSubobject<USplineComponent>("Sawblade Path");
	SawbladePath->SetupAttachment(DefaultRoot);
}

void ASawblade::BeginPlay()
{
	Super::BeginPlay();

	Sprite->OnComponentBeginOverlap.AddDynamic(this, &ASawblade::OnComponentBeginOverlap);
}

void ASawblade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bActivated)
	{
		CurrentTime = GetModifiedCurrentTime(DeltaTime);
		Sprite->SetWorldLocation(SawbladePath->GetLocationAtTime(CurrentTime, ESplineCoordinateSpace::World));
	}
}

void ASawblade::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PlayerDeathFX, PCharacter->GetActorLocation());
		UGameplayStatics::PlaySound2D(this, PlayerDeathSound);

		PCharacter->OnDeath();
	}
	else if (OtherActor->ActorHasTag("Box"))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BoxShredFX, OtherActor->GetActorLocation());
		UGameplayStatics::PlaySound2D(this, BoxShredSound);

		OtherActor->Destroy();
	}
}

float ASawblade::GetModifiedCurrentTime(float DeltaTime)
{
	float Out = 0.f;

	switch (SawPathType)
	{
		case ESPT_Mod: Out = FMath::Fmod(CurrentTime + DeltaTime, SawbladePath->Duration); break;
		case ESPT_Once: Out = FMath::Clamp(CurrentTime + DeltaTime, 0, SawbladePath->Duration); break;
		case ESPT_PingPong:
		{
			Out = CurrentTime + (bDirection ? DeltaTime : -DeltaTime);

			if ((bDirection && Out > SawbladePath->Duration) || (!bDirection && Out < 0))
			{
				bDirection = !bDirection;
			}

			Out = FMath::Clamp(Out, 0, SawbladePath->Duration);

			break;
		}
	}

	return Out;
}

