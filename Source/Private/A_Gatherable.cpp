// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Gatherable.h"
#include "Components/WidgetComponent.h"
#include "A_PlayerCharacter.h"
#include "InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Item.h"
#include "GAS/Potions/Plant.h"

// Sets default values
AA_Gatherable::AA_Gatherable()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    Trigger->SetupAttachment(RootComponent);

    UsesLeft = 1;
}

// Called when the game starts or when spawned
void AA_Gatherable::BeginPlay()
{
    if (!Item)
    {
        Item = ItemClass.GetDefaultObject();
        Item->InteractActionText = FText::FromString("Gather");
    }

    Super::BeginPlay();
}

// Called every frame
void AA_Gatherable::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AA_Gatherable::Interact(AA_PlayerCharacter* InteractingPlayer)
{
    Super::Interact(InteractingPlayer);

    if (!Item)
        return;

    InteractingPlayer->Inventory->AddItem(Item);

    if (--UsesLeft <= 0)
    {
        Destroy();
    }
}

void AA_Gatherable::OnItemDropped(UItem* DroppedItem)
{
    Item = DroppedItem;
    ItemClass = Item->StaticClass();
    Mesh->SetStaticMesh(Item->DroppedMesh);

    if (UPlant* Plant = Cast<UPlant>(DroppedItem))
    {
        Mesh->SetMaterial(0, Plant->PlantMaterial);
    }
    else if (UPotion* Potion = Cast<UPotion>(DroppedItem))
    {
        UMaterialInstanceDynamic* MI = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(2), this);
        Mesh->SetMaterial(0, MI);
        MI->SetVectorParameterValue(TEXT("LiquidColor"), Potion->GetLiquidColor());
    }

    Item->InteractActionText = FText::FromString("Gather");
    OnItemDroppedBP(Item);
}
