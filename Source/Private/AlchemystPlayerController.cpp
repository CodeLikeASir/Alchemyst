// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlchemystPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "AlchemystCharacter.h"
#include "A_PlayerCharacter.h"
#include "Engine/CollisionProfile.h"
#include "Engine/World.h"

AAlchemystPlayerController::AAlchemystPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AAlchemystPlayerController::BeginPlay()
{
	MyCharacter = Cast<AA_PlayerCharacter>(GetPawn());
}

void AAlchemystPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AAlchemystPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AAlchemystPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AAlchemystPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("Clicked", IE_Released, this,&AAlchemystPlayerController::OnClicked);

	InputComponent->BindAxis("MoveUp", this, &AAlchemystPlayerController::MoveUp);
	InputComponent->BindAxis("MoveRight", this, &AAlchemystPlayerController::MoveRight);
}

void AAlchemystPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AAlchemystCharacter* MyPawn = Cast<AAlchemystCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AAlchemystPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AAlchemystPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AAlchemystPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AAlchemystPlayerController::MoveUp(float Value)
{
	if(AA_PlayerCharacter* MyPawn = Cast<AA_PlayerCharacter>(GetPawn()))
	{
		MyPawn->MoveUp(Value);
	}
}

void AAlchemystPlayerController::MoveRight(float Value)
{
	if(AA_PlayerCharacter* MyPawn = Cast<AA_PlayerCharacter>(GetPawn()))
	{
		MyPawn->MoveRight(Value);
	}
}

void AAlchemystPlayerController::OnClicked()
{
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjTypes;
	ObjTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

	FHitResult HitResult;
	if (GetHitResultUnderCursorForObjects(ObjTypes,true,HitResult))
	{
		if(AInteractible* HitObject = Cast<AInteractible>(HitResult.GetActor()))
		{
			if(MyCharacter->IsInRange(HitObject))
			{
				HitObject->Interact(MyCharacter);
			}
		}
	}
}