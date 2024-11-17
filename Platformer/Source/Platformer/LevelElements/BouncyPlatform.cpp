// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/BouncyPlatform.h"

#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ABouncyPlatform::ABouncyPlatform()
{
	Collider = CreateDefaultSubobject<UBoxComponent>("Collider");
	Collider->SetCollisionProfileName("BlockAll");
	SetRootComponent(Collider);

	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	Trigger->SetCollisionProfileName("OverlapAll");
	Trigger->SetupAttachment(Collider);
}

void ABouncyPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	Collider->OnComponentHit.AddDynamic(this, &ABouncyPlatform::OnComponentHit);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ABouncyPlatform::OnComponentBeginOverlap);
}

void ABouncyPlatform::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	float* ActorVelocityAlongNormal = ActorVelocities.Find(OtherActor);
	if (ActorVelocityAlongNormal == nullptr) return;

	FVector CurrentVelocity = OtherActor->GetVelocity() * BounceNormal.GetSafeNormal();
	FVector StoredVelocity = -(BounceNormal * (*ActorVelocityAlongNormal));

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		Player->GetCharacterMovement()->AddImpulse(StoredVelocity, true);
	}
	else if (OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(StoredVelocity - CurrentVelocity, NAME_None, true);
	}

	ActorVelocities.Remove(OtherActor);
}

void ABouncyPlatform::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		ActorVelocities.Add({ OtherActor, FVector::DotProduct(Player->GetCharacterMovement()->Velocity, BounceNormal) });
	}
	else if (OtherActor->GetRootComponent()->IsSimulatingPhysics())
	{
		ActorVelocities.Add({ OtherActor, FVector::DotProduct(OtherActor->GetVelocity(), BounceNormal) });
	}
}
