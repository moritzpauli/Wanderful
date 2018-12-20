// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleItemSpot.generated.h"

UCLASS()
class WANDERFUL_API APuzzleItemSpot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleItemSpot();
	UPROPERTY(EditAnywhere)
	AActor* ClueItem;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* PlacementHologram;
	UPROPERTY(EditAnywhere)
	bool bInPosition;
	UPROPERTY(EditAnywhere)
	bool bInView;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UMaterialInterface* OGMaterial;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool GetIsInPosition() { return bInPosition; };
	void SetInPosition();

	
	
};
