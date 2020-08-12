// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingStation.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GAS/Potions/Potion.h"
#include "GAS/Potions/Plant.h"
#include "A_PlayerCharacter.h"
#include "InventoryComponent.h"

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
	CurrentPlayer = InteractingPlayer;
	
	Super::Interact(InteractingPlayer);
}

UPotion* ACraftingStation::OnCraftingCompleted(TArray<UItem*> UsedItems)
{
	FTasteStruct CraftedTaste = FTasteStruct();

	for (UItem* Item : UsedItems)
	{
		if(UPlant* Plant = Cast<UPlant>(Item))
			CraftedTaste += Plant->Taste;
	}

	UPotion* CraftedPotion = NewObject<UPotion>();
	CraftedPotion->Taste = CraftedTaste;

	return CraftedPotion;
}
