// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleItemSpot.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APuzzleItemSpot::APuzzleItemSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bInPosition = false;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("MyMesh");
	bInView = false;
	
}

// Called when the game starts or when spawned
void APuzzleItemSpot::BeginPlay()
{
	OGMaterial = mesh->GetMaterial(0);
	Super::BeginPlay();
	mesh->SetCollisionProfileName(TEXT("QueryOnly"));
	mesh->SetMaterial(0,PlacementHologram);
	mesh->SetVisibility(false);
}

void APuzzleItemSpot::SetInPosition()
{
	mesh->SetMaterial(0, OGMaterial);
	mesh->SetVisibility(true);
	bInPosition = true;
}



// Called every frame
void APuzzleItemSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bInView) {
		mesh->SetVisibility(true);
	}
	if (!bInView && !bInPosition) {
		mesh->SetVisibility(false);
	}

}

