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


protected:


public:
	virtual void Tick(float DeltaTime) override;

protected:
	void BeginPlay() override;
	
	
};
