// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Gatherable.h"
#include "Components/CapsuleComponent.h" 	
#include "Components/WidgetComponent.h"
#include "A_PlayerCharacter.h"
#include "InventoryComponent.h"
#include "Item.h"

// Sets default values
AA_Gatherable::AA_Gatherable()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	UsesLeft = 1;
}

// Called when the game starts or when spawned
void AA_Gatherable::BeginPlay()
{
	Super::BeginPlay();

	Item = ItemClass.GetDefaultObject();
}

// Called every frame
void AA_Gatherable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AA_Gatherable::Interact(AA_PlayerCharacter* InteractingPlayer)
{
	Super::Interact(InteractingPlayer);
	
	if(!Item)
		return;
	
	InteractingPlayer->Inventory->AddItem(Item);

	if(--UsesLeft <= 0)
	{
		Destroy();
	}
}
