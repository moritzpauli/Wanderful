// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUp.h"


// Sets default values
APickUp::APickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp,Warning, TEXT("I want muh console"));
	
}

// Called every frame
void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//picking up the stick
	if (pickedUP) {
		mesh->SetSimulatePhysics(false);
		AttachToComponent();
	}
	if (!pickedUP) {
		mesh->SetSimulatePhysics(true);
	}

}

