// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Enemy.h"
#include "LootBox.h"
#include "Kismet/GameplayStatics.h"
#include "InventoryComponent.h"

AA_Enemy::AA_Enemy()
{
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
    InventoryComponent->Capacity = 20;
}

void AA_Enemy::OnDeath()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s died."), *GetName()));
    
    FTransform SpawnTransform = GetTransform();
    SpawnTransform.SetScale3D(FVector(1.f));
    
    ALootBox* SpawnedLoot = Cast<ALootBox>(
        UGameplayStatics::BeginDeferredActorSpawnFromClass(this, LootBoxClass, SpawnTransform));
    if (SpawnedLoot != nullptr)
    {
        SpawnedLoot->InitLoot(InventoryComponent->Items);
        UGameplayStatics::FinishSpawningActor(SpawnedLoot, SpawnTransform);
    }

    Destroy();
}
