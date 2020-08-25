// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Potions/Potion_ThrownActor.h"

#include "AlchemystCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GAS/Potions/Potion.h"
#include "GameplayEffect.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APotion_ThrownActor::APotion_ThrownActor()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->OnComponentHit.AddDynamic(this, &APotion_ThrownActor::OnCompHit);
    RootComponent = Mesh;

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
    
    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(this);

    /*
    UGameplayStatics::ApplyRadialDamage(this, 10.f, GetActorLocation(),
        HitRadius, nullptr, IgnoredActors, this,
        GetInstigatorController(), true);
    */
    
    DrawDebugSphere(GetWorld(), GetActorLocation(), HitRadius, 12, FColor::Red, false, 2.0f, 0, 1.0f);

    // START
    TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
    traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

    // Ignore any specific actors
    TArray<AActor*> ignoreActors;
    // Ignore self or remove this line to not ignore any
    ignoreActors.Init(this, 1);

    // Array of actors that are inside the radius of the sphere
    TArray<AActor*> OutActors;
    
    UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), HitRadius, traceObjectTypes, AAlchemystCharacter::StaticClass(), ignoreActors, OutActors);

    // Optional: Use to have a visual representation of the SphereOverlapActors
    // DrawDebugSphere(GetWorld(), GetActorLocation(), radius, 12, FColor::Red, true, 10.0f);

    // Finally iterate over the outActor array
    for (AActor* OverlappedActor : OutActors) {
        if(AAlchemystCharacter* Character = Cast<AAlchemystCharacter>(OverlappedActor))
        {
            UE_LOG(LogTemp, Log, TEXT("OverlappedCharacter: %s"), *Character->GetName());
            Character->ApplyGameplayEffect(AppliedEffect.GetDefaultObject());
        }
    }
    // END
    
    URadialForceComponent* ForceComp = NewObject<URadialForceComponent>( this, TEXT( "MissileHitForceComp" ) );
    ForceComp->RegisterComponent();
    ForceComp->SetWorldLocation( Hit.Location );
    ForceComp->SetWorldRotation( Hit.Location.Rotation() );
     
    ForceComp->Radius = ThrownPotion->ImpactRadius;
    ForceComp->ForceStrength = 100000;
    ForceComp->ImpulseStrength = 100000;
     
    ForceComp->SetWorldLocation( Hit.Location );
     
    //ForceComp->AttachTo( this->GetRootComponent(), NAME_None, EAttachLocation::KeepWorldPosition );
    ForceComp->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
     
    ForceComp->FireImpulse();
     
    // Now simply remove it, it will automatically drop the component from list returned by this->GetComponents().
    ForceComp->DestroyComponent( true );

    Destroy();
}

void APotion_ThrownActor::InitThrownPotion(UPotion* Potion, float PlayerSpeed)
{
    ThrownPotion = Potion;

    ProjectileMovement->MaxSpeed = PlayerSpeed + 500.f;
    ProjectileMovement->InitialSpeed = PlayerSpeed + 500.f;

    HitRadius = 200.f; //Potion->ImpactRadius;
}
