// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItem.h"
#include "Kismet/GameplayStatics.h"
#include "A_PlayerCharacter.h"
#include "Alch_RangedWeapon.h"

void UWeaponItem::Use(AA_PlayerCharacter* Character)
{
    FTransform SpawnTransform = Character->WeaponPosition->GetComponentTransform();
    AActor* SpawnedActor = Cast<AActor>( UGameplayStatics::BeginDeferredActorSpawnFromClass(this, WeaponActor, SpawnTransform));
    if (SpawnedActor != nullptr)
    {
        SpawnedActor->AttachToComponent(Character->WeaponPosition, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        SpawnedActor->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("RightHand_Socket")));
        UGameplayStatics::FinishSpawningActor(SpawnedActor, SpawnTransform);
    }
}
