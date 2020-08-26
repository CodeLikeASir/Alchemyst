// Fill out your copyright notice in the Description page of Project Settings.


#include "Alch_RangedWeapon.h"

// Sets default values
AAlch_RangedWeapon::AAlch_RangedWeapon()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = StaticMeshComponent;
}

// Called when the game starts or when spawned
void AAlch_RangedWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}