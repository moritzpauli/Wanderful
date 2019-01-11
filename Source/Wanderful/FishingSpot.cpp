// Fill out your copyright notice in the Description page of Project Settings.

#include "FishingSpot.h"
#include "Components/StaticMeshComponent.h"

AFishingSpot::AFishingSpot() {
	Fishtank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyFishtank"));
}


void AFishingSpot::BeginPlay()
{
	Super::BeginPlay();
	myMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}



void AFishingSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetBInteract()) {
		myMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (!GetBInteract()) {
		myMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

}

