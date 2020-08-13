// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "A_PlayerCharacter.h"
#include "Item.h"
#include "GAS/Potions/Potion.h"
#include "GAS/Potions/GA_Potion_Throw.h"
#include "AbilitySystemComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	Capacity = 20;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for(auto& Item : DefaultItems)
	{
		AddItem(Item);
	}
	
	if(AA_PlayerCharacter* PlayerCharacter = Cast<AA_PlayerCharacter>(GetOwner()))
	{
		OwningPlayer = PlayerCharacter;
	}
}

bool UInventoryComponent::AddItem(UItem* Item)
{
	if(Items.Num() >= Capacity || !Item)
	{
		return false;
	}

	Item->OwningInventory = this;
	Item->World = GetWorld();
	Items.Add(Item);

	if(UPotion* Potion = Cast<UPotion>(Item))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Collectd a potion")));
		if(Potion->ThrowAbility)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("It seems magical")));
			OwningPlayer->GrantAbility(Potion->ThrowAbility);
		}
	}

	// Update UI
	OnInventoryUpdated.Broadcast();

	return true;
}

bool UInventoryComponent::RemoveItem(UItem* Item)
{
	if(Item)
	{
		Item->OwningInventory = nullptr;
		Item->World = nullptr;
		Items.RemoveSingle(Item);
		OnInventoryUpdated.Broadcast();

		if(OwningPlayer->EquippedItem == Item)
		{
			OwningPlayer->EquippedItem = nullptr;
		}
		
		return true;
	}

	return false;
}

