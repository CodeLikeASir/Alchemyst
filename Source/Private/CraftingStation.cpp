// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingStation.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GAS/Potions/Potion.h"
#include "GAS/Potions/Plant.h"
#include "A_PlayerCharacter.h"
#include "InventoryComponent.h"
#include "GAS/Potions/GA_Potion_Throw.h"

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
		{
			CraftedTaste += Plant->Taste;
		}
	}

	UPotion* CraftedPotion = NewObject<UPotion>();
	CraftedPotion->Taste = CraftedTaste;
	
	if(UPlant* Plant = Cast<UPlant>(UsedItems[0]))
		CraftedPotion->SetLiquidColor(Plant->LiquidColor);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Giving potion an ability")));
	CraftedPotion->ThrowAbility = UGA_Potion_Throw::StaticClass();

	return CraftedPotion;
}
