// Fill out your copyright notice in the Description page of Project Settings.


#include "A_PlayerCharacter.h"

#include "A_Gatherable.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Item.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "InventoryComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/Potions/Potion.h"
#include "GAS/Potions/GA_Potion_Consume.h"
#include "GAS/Potions/GA_Potion_Throw.h"

AA_PlayerCharacter::AA_PlayerCharacter()
{
    // Create a camera boom...
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
    CameraBoom->TargetArmLength = 800.f;
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

    // Create a camera...
    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Create a decal in the world to show the cursor's location
    CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
    CursorToWorld->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(
        TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
    if (DecalMaterialAsset.Succeeded())
    {
        CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
    }
    CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
    CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

    // Activate ticking in order to update the cursor every frame.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
    
    InteractableCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Interactable"));
    InteractableCollision->InitSphereRadius(150.f);
    InteractableCollision->OnComponentBeginOverlap.AddDynamic(this, &AA_PlayerCharacter::OnInteractableOverlap);
    InteractableCollision->OnComponentEndOverlap.AddDynamic(this, &AA_PlayerCharacter::OnEndInteractableOverlap);
    InteractableCollision->SetupAttachment(RootComponent);

    Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
    Inventory->Capacity = 20;

    ThrowPosition = CreateDefaultSubobject<USphereComponent>(TEXT("ThrowPosition"));
    ThrowPosition->SetupAttachment(RootComponent);
}

void AA_PlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (CursorToWorld != nullptr && !bDisableCursor)
    {
        if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
        {
            if (UWorld* World = GetWorld())
            {
                FHitResult HitResult;
                FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
                FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
                FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
                Params.AddIgnoredActor(this);
                World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
                FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
                CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
            }
        }
        else if (APlayerController* PC = Cast<APlayerController>(GetController()))
        {
            FHitResult TraceHitResult;
            PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
            FVector CursorFV = TraceHitResult.ImpactNormal;
            FRotator CursorR = CursorFV.Rotation();
            CursorToWorld->SetWorldLocation(TraceHitResult.Location);
            CursorToWorld->SetWorldRotation(CursorR);
        }
    }
}

void AA_PlayerCharacter::SetEquippedItem(UItem* NewEquippedItem)
{
    if (UPotion* Potion = Cast<UPotion>(EquippedItem))
    {
        if (Potion->ThrowAbility)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Removed %s"), *Potion->ThrowAbility->GetName()));
            RemoveAbility(Potion->ThrowAbility);
        }
    }
    
    EquippedItem = NewEquippedItem;

    if (UPotion* Potion = Cast<UPotion>(NewEquippedItem))
    {
        if (Potion->ThrowAbility)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Grant %s"), *Potion->ThrowAbility->GetName()));
            GrantAbility(Potion->ThrowAbility);
        }
    }
}

void AA_PlayerCharacter::MoveUp(float Value)
{
    AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
}

void AA_PlayerCharacter::MoveRight(float Value)
{
    AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
}

void AA_PlayerCharacter::OnInteractableOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                               const FHitResult& SweepResult)
{
    if (AInteractible* Interactible = Cast<AInteractible>(OtherActor))
    {
        Interactible->ToggleInteractUI(true);
        ObjectsInRange.Add(Interactible);
    }
}

void AA_PlayerCharacter::OnEndInteractableOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (AInteractible* Interactible = Cast<AInteractible>(OtherActor))
    {
        Interactible->ToggleInteractUI(false);
        ObjectsInRange.Remove(Interactible);
    }
}

bool AA_PlayerCharacter::IsInRange(AInteractible* HitObject)
{
    return ObjectsInRange.Contains(HitObject);
}

FVector AA_PlayerCharacter::GetThrowPos()
{
    return ThrowPosition->GetComponentLocation();
}

void AA_PlayerCharacter::UseItem(UItem* Item)
{
    if (Item)
    {
        Item->Use(this);
        Item->OnUse(this); // BP event
    }
}

void AA_PlayerCharacter::RemoveItem(UItem* Item)
{
    if(Item == EquippedItem)
    {

        if (UPotion* Potion = Cast<UPotion>(EquippedItem))
        {
            if (Potion->ThrowAbility)
            {
                RemoveAbility(Potion->ThrowAbility);
            }
        }
        
        EquippedItem = nullptr;
    }

    Inventory->RemoveItem(Item);
}

void AA_PlayerCharacter::SetDisableCursor(bool Value)
{
    bDisableCursor = Value;
    CursorToWorld->SetVisibility(!Value);
}
