// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Potions/Potion_ThrownActor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
APotion_ThrownActor::APotion_ThrownActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	HitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HitSphere"));
}

// Called when the game starts or when spawned
void APotion_ThrownActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APotion_ThrownActor::InitThrownPotion(UPotion* Potion, UStaticMesh* PotionMesh, float HitRadius,
	UNiagaraSystem* PotionHitFX)
{
	ThrownPotion = Potion;
	Mesh->SetStaticMesh(PotionMesh);
	HitSphere->SetSphereRadius(HitRadius);
	HitFX = PotionHitFX;
}
