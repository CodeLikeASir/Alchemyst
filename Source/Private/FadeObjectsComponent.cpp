// Fill out your copyright notice in the Description page of Project Settings.

#include "FadeObjectsComponent.h"

#include "Kismet/GameplayStatics.h"

UFadeObjectsComponent::UFadeObjectsComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    bEnable = true;

    AddObjectInterval = 0.1f;
    CalcFadeInterval = 0.05f;

    FadeRate = 10.0f;

    CapsuleHalfHeight = 88.0f;
    CapsuleRadius = 34.0f;

    WorkDistance = 5000.0f;
    NearCameraRadius = 300.0f;

    NearObjectFade = 0.3;
    FarObjectFade = 0.1;

    ImmediatelyFade = 0.5f;

    // Add first collision type
    ObjectTypes.Add(ECC_WorldStatic);

    CurrentFade = 0.0f;
}

// Called when the game starts
void UFadeObjectsComponent::BeginPlay()
{
    Super::BeginPlay();
    
    if (bEnable)
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_AddObjectsTimer, this,
                                               &UFadeObjectsComponent::AddObjectsToHide,
                                               AddObjectInterval, true);
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_ObjectComputeTimer, this,
                                               &UFadeObjectsComponent::FadeObjWorker,
                                               CalcFadeInterval, true);
    }
}

// Called every frame
void UFadeObjectsComponent::TickComponent(const float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UFadeObjectsComponent::AddObjectsToHide()
{
    UGameplayStatics::GetAllActorsOfClass(this, PlayerClass, CharacterArray);

    for (AActor* CurrentActor : CharacterArray)
    {
        const FVector TraceStart = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->
                                            GetCameraLocation();
        const FVector TraceEnd = CurrentActor->GetActorLocation();
        const FRotator TraceRot = CurrentActor->GetActorRotation();
        FVector TraceLength = TraceStart - TraceEnd;
        const FQuat AcQuat = CurrentActor->GetActorQuat();

        if (TraceLength.Size() < WorkDistance)
        {
            FCollisionQueryParams TraceParams(TEXT("FadeObjectsTrace"), true, GetOwner());

            TraceParams.AddIgnoredActors(ActorsIgnore);
            //traceParams.bTraceAsyncScene = true;
            TraceParams.bReturnPhysicalMaterial = false;
            // Not tracing complex uses the rough collision instead making tiny objects easier to select.
            TraceParams.bTraceComplex = false;

            TArray<FHitResult> HitArray;
            TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

            // Convert ECollisionChannel to ObjectType
            for (int i = 0; i < ObjectTypes.Num(); ++i)
            {
                TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ObjectTypes[i].GetValue()));
            }

            // Check distance between camera and player for new object to fade, and add this in array
            GetWorld()->SweepMultiByObjectType(HitArray, TraceStart, TraceEnd, AcQuat, TraceObjectTypes,
                                               FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
                                               TraceParams);

            for (int HitIndex = 0; HitIndex < HitArray.Num(); ++HitIndex)
            {
                if (HitArray[HitIndex].bBlockingHit && IsValid(HitArray[HitIndex].GetComponent()) && !FadeObjectsHit.Contains(
                    HitArray[HitIndex].GetComponent()))
                {
                    FadeObjectsHit.AddUnique(HitArray[HitIndex].GetComponent());
                }
            }
        }
    }

    // Make fade array after complete GetAllActorsOfClass loop
    for (int fO = 0; fO < FadeObjectsHit.Num(); ++fO)
    {
        // If not contains this component in fadeObjectsTemp
        if (!FadeObjectsTemp.Contains(FadeObjectsHit[fO]))
        {
            TArray<UMaterialInterface*> lBaseMaterials;
            TArray<UMaterialInstanceDynamic*> lMidMaterials;

            lBaseMaterials.Empty();
            lMidMaterials.Empty();

            FadeObjectsTemp.AddUnique(FadeObjectsHit[fO]);

            // For loop all materials ID in object
            for (int nM = 0; nM < FadeObjectsHit[fO]->GetNumMaterials(); ++nM)
            {
                lMidMaterials.Add(UMaterialInstanceDynamic::Create(FadeMaterial, FadeObjectsHit[fO]));
                lBaseMaterials.Add(FadeObjectsHit[fO]->GetMaterial(nM));

                // Set new material on object
                FadeObjectsHit[fO]->SetMaterial(nM, lMidMaterials.Last());
            }
            // Create new fade object in array of objects to fade
            FFadeObjStruct newObject;
            newObject.NewElement(FadeObjectsHit[fO], lBaseMaterials, lMidMaterials, ImmediatelyFade, true);
            // Add object to array
            FadeObjects.Add(newObject);

            // Set collision on Primitive Component
            FadeObjectsHit[fO]->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
        }
    }

    // Set hide to visible true if contains
    for (int fOT = 0; fOT < FadeObjectsTemp.Num(); ++fOT)
    {
        if (!FadeObjectsHit.Contains(FadeObjectsTemp[fOT]))
        {
            FadeObjects[fOT].SetHideOnly(false);
        }
    }

    // Clear array
    FadeObjectsHit.Empty();
}

void UFadeObjectsComponent::FadeObjWorker()
{
    if (FadeObjects.Num() > 0)
    {
        // For loop all fade objects
        for (int i = 0; i < FadeObjects.Num(); ++i)
        {
            // Index of iteration
            int fnID = i;

            float adaptiveFade;

            if (fnID == FadeObjects.Num())
            {
                adaptiveFade = NearObjectFade;
            }
            else
            {
                adaptiveFade = FarObjectFade;
            }

            // For loop FadeMID array
            for (int t = 0; t < FadeObjects[i].FadeMID.Num(); ++t)
            {
                float targetF;

                const float currentF = FadeObjects[i].FadeCurrent;

                if (FadeObjects[i].bToHide)
                {
                    targetF = adaptiveFade;
                }
                else
                {
                    targetF = 1.0f;
                }

                const float newFade = FMath::FInterpConstantTo(currentF, targetF, GetWorld()->GetDeltaSeconds(),
                                                               FadeRate);

                FadeObjects[i].FadeMID[t]->SetScalarParameterValue("Fade", newFade);

                CurrentFade = newFade;

                if(i < FadeObjects.Num() && newFade)
                    FadeObjects[i].SetFadeAndHide(newFade, FadeObjects[i].bToHide);
            }
            // remove index in array
            if (CurrentFade == 1.0f)
            {
                for (int bmi = 0; bmi < FadeObjects[fnID].BaseMatInterface.Num(); ++bmi)
                {
                    FadeObjects[fnID].PrimitiveComp->SetMaterial(bmi, FadeObjects[fnID].BaseMatInterface[bmi]);
                }

                FadeObjects[fnID].PrimitiveComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
                FadeObjects.RemoveAt(fnID);
                FadeObjectsTemp.RemoveAt(fnID);
            }
        }
    }
}

void UFadeObjectsComponent::SetActivate()
{
    bEnable = true;
}

void UFadeObjectsComponent::SetDeactivate()
{
    bEnable = false;
}
