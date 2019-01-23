// Fill out your copyright notice in the Description page of Project Settings.

#include "CogPlacementPuzzle.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

ACogPlacementPuzzle::ACogPlacementPuzzle() {
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>("MainMesh");
	cog1= CreateDefaultSubobject<UStaticMeshComponent>("cog1");
	cog2 = CreateDefaultSubobject<UStaticMeshComponent>("cog2");
	cog3 = CreateDefaultSubobject<UStaticMeshComponent>("cog3");
}



void ACogPlacementPuzzle::BeginPlay()
{
	Super::BeginPlay();
	PuzzleSpots = GetPuzzleSpots();
}

void ACogPlacementPuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetSolvedStatus()) {
		UE_LOG(LogTemp, Warning, TEXT("cogpuzzle solved"));
		cog1->AddLocalRotation(AddedRotation*DeltaTime,false,0,ETeleportType::None);
		cog2->AddLocalRotation(AddedRotation*DeltaTime, false, 0, ETeleportType::None);
		cog3->AddLocalRotation(AddedRotation*DeltaTime, false, 0, ETeleportType::None);
		for (int i = 0; i < PuzzleSpots.Num(); i++) {
			PuzzleSpots[i]->AddActorLocalRotation(AddedRotation*DeltaTime, false, 0, ETeleportType::None);
		}
	}
}
