// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingStation.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ACraftingStation::ACraftingStation()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACraftingStation::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACraftingStation::Interact(AA_PlayerCharacter* InteractingPlayer)
{
	Super::Interact(InteractingPlayer);
}
