// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Potion_ThrownActor.generated.h"

UCLASS()
class ALCHEMYST_API APotion_ThrownActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    APotion_ThrownActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Potion")
    class UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Potion")
    class UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Potion")
    class UNiagaraSystem* HitFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Potion")
    class UPotion* ThrownPotion;

    UFUNCTION()
    void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                   FVector NormalImpulse, const FHitResult& Hit);

    float HitRadius;

public:
    UFUNCTION(BlueprintCallable)
    void InitThrownPotion(UPotion* Potion, float PlayerSpeed);

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UGameplayEffect> AppliedEffect;
};
