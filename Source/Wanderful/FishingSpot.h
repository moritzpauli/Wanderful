// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaticInteractable.h"
#include "FishingSpot.generated.h"

/**
 * 
 */
UCLASS()
class WANDERFUL_API AFishingSpot : public AStaticInteractable
{
	GENERATED_BODY()

public:
	AFishingSpot();
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Fishtank;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Float;
	bool bTankInView;
	UPROPERTY(EditAnywhere)
	FVector FloatPosition;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* FishtankPlane;
	UPROPERTY(EditAnywhere)
	class USceneComponent* WireConnector;



protected:
	bool SetHeight;
	FTransform PlaneTransform;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	void BeginPlay() override;
	
	
};
