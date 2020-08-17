// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FadeObjectsComponent.generated.h"

USTRUCT()
struct FFadeObjStruct
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    UPrimitiveComponent* PrimitiveComp;
    UPROPERTY()
    TArray<UMaterialInterface*> BaseMatInterface;
    UPROPERTY()
    TArray<UMaterialInstanceDynamic*> FadeMID;
    UPROPERTY()
    float FadeCurrent;
    UPROPERTY()
    bool bToHide;

    void NewElement(UPrimitiveComponent* NewComponent, TArray<UMaterialInterface*> NewBaseMat,
                    TArray<UMaterialInstanceDynamic*> newMID, float currentFade, bool bHide)
    {
        PrimitiveComp = NewComponent;
        BaseMatInterface = NewBaseMat;
        FadeMID = newMID;
        FadeCurrent = currentFade;
        bToHide = bHide;
    }

    void SetHideOnly(bool Hide)
    {
        bToHide = Hide;
    }

    void SetFadeAndHide(float newFade, bool newHide)
    {
        FadeCurrent = newFade;
        bToHide = newHide;
    }

    //For Destroy
    void Destroy()
    {
        PrimitiveComp = nullptr;
    }

    //Constructor
    FFadeObjStruct()
    {
        PrimitiveComp = nullptr;
        FadeCurrent = 0;
        bToHide = true;
    }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMYST_API UFadeObjectsComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    // Sets default values for this component's properties
    UFadeObjectsComponent();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    // Check objects between camera manager and character and add to array for fade
    void AddObjectsToHide();

    // Fade worker.
    void FadeObjWorker();

    // Enable or disable fade object worker
    void SetActivate();

    void SetDeactivate();

private:

    TArray<FFadeObjStruct> FadeObjects;

    // Some worker timer
    FTimerHandle TimerHandle_ObjectComputeTimer;

    FTimerHandle TimerHandle_AddObjectsTimer;

    // Temp variable
    float CurrentFade;

    // Now ID
    int32 FadeNowID;

    // Primitive components temp variable
    TArray<UPrimitiveComponent*> FadeObjectsTemp;

    // Primitive components temp variable
    TArray<UPrimitiveComponent*> FadeObjectsHit;

    // Translucent material for fade object
    UPROPERTY(EditAnywhere, Category = "Fade Objects")
    UMaterial* FadeMaterial;

    // Enable or disable fade object worker
    UPROPERTY(EditAnywhere, Category = "Fade Objects")
    bool bEnable;

    // Timer interval
    float AddObjectInterval;
    float CalcFadeInterval;

    UPROPERTY(EditAnywhere, Category = "Fade Objects")
    float WorkDistance;
    UPROPERTY(EditAnywhere, Category = "Fade Objects")
    float NearCameraRadius;

    UPROPERTY(EditAnywhere, Category = "Fade Objects")
    UClass* PlayerClass;

    // Check trace block by this
    UPROPERTY(EditAnywhere, Category = "Fade Objects")
    TArray<TEnumAsByte<ECollisionChannel>> ObjectTypes;

    UPROPERTY(EditAnywhere, Category = "Fade Objects")
    TArray<AActor*> ActorsIgnore;

    // Rate fade increment
    UPROPERTY(EditAnywhere, Category = "Fade Objects")
    float FadeRate;

    // Trace object size
    UPROPERTY(EditAnywhere, Category = "Fade Objects")
    float CapsuleHalfHeight;
    // Trace object size
    UPROPERTY(EditAnywhere, Category = "Fade Objects")
    float CapsuleRadius;

    // All characters array (maybe you control ( > 1 ) characters)
    UPROPERTY()
    TArray<AActor*> CharacterArray;

    // Fade near and close parameters
    UPROPERTY(EditAnywhere, Category = "Fade Objects")
    float NearObjectFade;
    // Fade near and close parameters
    UPROPERTY(EditAnywhere, Category = "Fade Objects")
    float FarObjectFade;

    // Instance fade
    UPROPERTY(EditAnywhere, Category = "Fade Objects")
    float ImmediatelyFade;
};
