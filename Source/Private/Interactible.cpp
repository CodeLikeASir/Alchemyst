// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactible.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AInteractible::AInteractible()
{
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->SetSphereRadius(100.f);
	Trigger->SetCollisionObjectType(ECC_GameTraceChannel1);
	RootComponent = Trigger;
	
	InteractUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractUI"));
	InteractUI->SetupAttachment(Trigger);
	InteractUI->SetDrawSize(FVector2D(200, 100));
	InteractUI->SetVisibility(false);
}

void AInteractible::ToggleInteractUI(bool bIsVisible)
{
	InteractUI->SetVisibility(bIsVisible);
}

void AInteractible::Interact(AA_PlayerCharacter* InteractingPlayer)
{
	InteractBP(InteractingPlayer);
}