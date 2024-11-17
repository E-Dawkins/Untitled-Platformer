// Fill out your copyright notice in the Description page of Project Settings.


#include "Spike.h"

#include "PaperSpriteComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASpike::ASpike()
{
	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>("Box Collider");
	Sprite->SetupAttachment(RootComponent);
}

void ASpike::BeginPlay()
{
	Super::BeginPlay();
	
	Sprite->OnComponentBeginOverlap.AddDynamic(this, &ASpike::OnComponentBeginOverlap);
}

void ASpike::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PlayerDeathFX, PCharacter->GetActorLocation());
		UGameplayStatics::PlaySound2D(this, PlayerDeathSound);

		PCharacter->OnDeath();
	}
}

