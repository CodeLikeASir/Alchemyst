// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlchemystCharacter.h"
#include "A_Gatherable.h"
#include "A_PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMYST_API AA_PlayerCharacter : public AAlchemystCharacter
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Component")
    class USphereComponent* InteractableCollision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Component")
    class USceneComponent* ThrowPosition;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<class AInteractible*> ObjectsInRange;

    public:   
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Component")
    class USceneComponent* WeaponPosition;
    
    /** Top down camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* TopDownCameraComponent;

    /** Camera boom positioning the camera above the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /** A decal that projects to the cursor location. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UDecalComponent* CursorToWorld;

    /** Returns TopDownCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns CursorToWorld subobject **/
    FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
    
    AA_PlayerCharacter();

    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Player")
    class UItem* EquippedItem;

    UItem* GetEquippedItem() const { return EquippedItem; }

    UFUNCTION(BlueprintCallable)
    void SetEquippedItem(UItem* NewEquippedItem);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Component")
    class UInventoryComponent* Inventory;

    void MoveUp(float Value);
    void MoveRight(float Value);

    UFUNCTION()
    void OnInteractableOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                               const FHitResult& SweepResult);

    UFUNCTION()
    void OnEndInteractableOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    bool IsInRange(AInteractible* HitObject);

    FVector GetThrowPos();

    UFUNCTION(BlueprintCallable, Category= "Items")
    void UseItem(class UItem* Item);

    void RemoveItem(class UItem* Item);

    bool bDisableCursor;

    UFUNCTION(BlueprintCallable)
    void SetDisableCursor(bool Value);

    UFUNCTION(BlueprintCallable)
    FTransform GetShootRotation();
};
