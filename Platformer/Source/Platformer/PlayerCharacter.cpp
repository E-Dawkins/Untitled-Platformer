// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "PaperSpriteComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CircleFadeUserWidget.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->DestroyComponent();

	PlayerSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Player Sprite");
	PlayerSprite->SetupAttachment(RootComponent);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (TransitionWidgetClass)
	{
		TransitionWidgetPtr = CreateWidget<UCircleFadeUserWidget>(GetWorld()->GetFirstPlayerController(), TransitionWidgetClass);
		if (TransitionWidgetPtr)
		{
			TransitionWidgetPtr->AddToViewport();
			FTimerDelegate TransitionDelegate;
			TransitionDelegate.BindUObject(this, &APlayerCharacter::SetupTransitionWidget, true);
			GetWorld()->GetTimerManager().SetTimer(TransitionHandle, TransitionDelegate, 0.2f, false);
		}
	}

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(TransitionHandle);
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CurrentJumpBuffer = FMath::Clamp(CurrentJumpBuffer - DeltaSeconds, 0.f, JumpBuffer);
	CurrentCoyoteTime = FMath::Clamp(CurrentCoyoteTime - DeltaSeconds, 0.f, CoyoteTime);

	if (CanJump() && CurrentJumpBuffer > 0.f)
	{
		TryJump();
		CurrentJumpBuffer = 0.f;
	}
}

void APlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	// weren't falling, now we are ... start 'coyote time'
	if (PrevMovementMode != EMovementMode::MOVE_Falling && GetCharacterMovement()->GetMovementName() == "Falling")
	{
		CurrentCoyoteTime = CoyoteTime;
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::TryJump);
	PlayerInputComponent->BindAction("Restart", IE_Pressed, this, &APlayerCharacter::OnInput_Restart);
	PlayerInputComponent->BindAction("Exit", IE_Pressed, this, &APlayerCharacter::OnInput_Exit);
}

void APlayerCharacter::OnDeath()
{
	// Stop player movement / collision
	RootComponent->SetVisibility(false, true);
	GetCharacterMovement()->DisableMovement();
	DisableInput(GetWorld()->GetFirstPlayerController());
	SetActorEnableCollision(false);

	// Restart the level after the transition anim
	TransitionWidgetPtr->SetCirclePositionWorld(GetActorLocation());
	TransitionWidgetPtr->PlayCircleShrink();

	FWidgetAnimationDynamicEvent Event;
	Event.BindDynamic(this, &APlayerCharacter::RestartLevel);
	TransitionWidgetPtr->BindToCircleAnimFinish(Event);
}

void APlayerCharacter::MoveRight(float Value)
{
	AddMovementInput(FVector::XAxisVector, Value);

	if (!FMath::IsNearlyZero(Value)) // check we are actually moving
	{
		PlayerSprite->SetRelativeRotation(Value > 0 ? FRotator::ZeroRotator : FRotator(0, 180, 0));
	}
}

void APlayerCharacter::TryJump()
{
	if (CanJump())
	{
		CalculateJumpVelocity();
		Jump();

		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Jump");
	}
	else
	{
		if (CurrentCoyoteTime > 0.f) // within 'coyote time' threshold
		{
			LaunchCharacter(FVector::UpVector * GetCharacterMovement()->JumpZVelocity, false, true);
			CurrentCoyoteTime = 0.f;
		}
		else
		{
			CurrentJumpBuffer = JumpBuffer;
		}
	}
}

void APlayerCharacter::CalculateJumpVelocity() const
{
	float Gravity = GetCharacterMovement()->GetGravityZ();
	float JumpVelocity = FMath::Sqrt(-2 * Gravity * JumpHeight);

	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
}

void APlayerCharacter::OnInput_Restart()
{
	SetupTransitionWidget(false);

	FWidgetAnimationDynamicEvent Event;
	Event.BindDynamic(this, &APlayerCharacter::RestartLevel);
	TransitionWidgetPtr->BindToCircleAnimFinish(Event);
}

void APlayerCharacter::OnInput_Exit()
{
	SetupTransitionWidget(false);

	FWidgetAnimationDynamicEvent Event;
	Event.BindDynamic(this, &APlayerCharacter::ExitToMainMenu);
	TransitionWidgetPtr->BindToCircleAnimFinish(Event);
}

void APlayerCharacter::SetupTransitionWidget(bool bGrow)
{
	TransitionWidgetPtr->SetCirclePositionWorld(GetActorLocation());

	if (bGrow) TransitionWidgetPtr->PlayCircleGrow();
	else TransitionWidgetPtr->PlayCircleShrink();
}

void APlayerCharacter::RestartLevel()
{
	GetWorld()->GetFirstPlayerController()->RestartLevel();
}

void APlayerCharacter::ExitToMainMenu()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MainMenuLevel);
}
