// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Potions/Potion_ThrownActor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

// Sets default values
APotion_ThrownActor::APotion_ThrownActor()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->OnComponentHit.AddDynamic(this, &APotion_ThrownActor::OnCompHit);
    RootComponent = Mesh;

    HitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));
    HitSphere->SetupAttachment(Mesh);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void APotion_ThrownActor::BeginPlay()
{
    Super::BeginPlay();
}

void APotion_ThrownActor::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                    FVector NormalImpulse, const FHitResult& Hit)
{
    if (!HitFX)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Missing HitFX")));
        Destroy();
        return;
    }

    FRotator HitRotation = Hit.ImpactNormal.Rotation();
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        this, HitFX, Hit.ImpactPoint, HitRotation, FVector(1),
        true, true, ENCPoolMethod::AutoRelease, false);

    Destroy();
}

void APotion_ThrownActor::InitThrownPotion(UPotion* Potion, float PlayerSpeed)
{
    ThrownPotion = Potion;

    ProjectileMovement->MaxSpeed = PlayerSpeed + 500.f;
    ProjectileMovement->InitialSpeed = PlayerSpeed + 500.f;
}
