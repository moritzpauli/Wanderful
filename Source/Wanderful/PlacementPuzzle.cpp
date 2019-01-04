// Fill out your copyright notice in the Description page of Project Settings.

#include "PlacementPuzzle.h"



// Sets default values
APlacementPuzzle::APlacementPuzzle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bSolved = false;
	counter = 0;
}

// Called when the game starts or when spawned
void APlacementPuzzle::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("%d"), PuzzlePieces.Num());
	
}

// Called every frame
void APlacementPuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	if (!bSolved) {
		for (int i = 0; i < PuzzlePieces.Num(); i++) {
			if (PuzzlePieces[i]->bInPosition == true) {
				counter++;
				if (counter == PuzzlePieces.Num()) {
					bSolved = true;
				}
			}
		}
	}
	counter = 0;
}

