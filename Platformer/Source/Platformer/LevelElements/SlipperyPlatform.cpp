// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/SlipperyPlatform.h"

#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ASlipperyPlatform::ASlipperyPlatform()
{
	Collider = CreateDefaultSubobject<UBoxComponent>("Collider");
	Collider->SetCollisionProfileName("BlockAll");
	SetRootComponent(Collider);

	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	Trigger->SetCollisionProfileName("OverlapAll");
	Trigger->SetupAttachment(Collider);
}

void ASlipperyPlatform::BeginPlay()
{
	Super::BeginPlay();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASlipperyPlatform::OnComponentBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ASlipperyPlatform::OnComponentEndOverlap);
}

void ASlipperyPlatform::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (UCharacterMovementComponent* MovementComp = Player->GetCharacterMovement())
		{
			MovementComp->GroundFriction = PlayerGroundFriction_Slippery;
			MovementComp->BrakingDecelerationWalking = PlayerDeceleration_Slippery;
		}
	}
}

void ASlipperyPlatform::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (UCharacterMovementComponent* MovementComp = Player->GetCharacterMovement())
		{
			MovementComp->GroundFriction = PlayerGroundFriction_Normal;
			MovementComp->BrakingDecelerationWalking = PlayerDeceleration_Normal;
		}
	}
}
