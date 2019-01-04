// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacementPuzzle.h"
#include "CogPlacementPuzzle.generated.h"

/**
 * 
 */
UCLASS()
class WANDERFUL_API ACogPlacementPuzzle : public APlacementPuzzle
{
	GENERATED_BODY()
	
public:
	ACogPlacementPuzzle();
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MainMesh;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* cog1;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* cog2;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* cog3;
	UPROPERTY(EditAnywhere)
	FRotator AddedRotation;


protected:
	virtual void BeginPlay() override;
	TArray<class APuzzleItemSpot*> PuzzleSpots;

public:
	virtual void Tick(float DeltaTime) override;
	
	
	
};
