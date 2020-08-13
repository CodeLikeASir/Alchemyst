// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AlchemystPlayerController.generated.h"

#define COLLISION_INTERACTABLE ECC_GameTraceChannel1

UCLASS()
class AAlchemystPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAlchemystPlayerController();
	virtual void BeginPlay() override;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	class AA_PlayerCharacter* MyCharacter;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void MoveUp(float Value);
	void MoveRight(float Value);

	void OnClicked();

	virtual void AcknowledgePossession(APawn* P) override;
};


