// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaticInteractable.h"
#include "FloodGate.generated.h"

/**
 * 
 */
UCLASS()
class WANDERFUL_API AFloodGate : public AStaticInteractable
{
	GENERATED_BODY()
	
public:
	AFloodGate();
	UPROPERTY(EditAnywhere)
	float Surface1ZOG;
	UPROPERTY(EditAnywhere)
	float Surface2ZOG;
	UPROPERTY(EditAnywhere)
	AActor* WaterSurface1;
	UPROPERTY(EditAnywhere)
	AActor* WaterSurface2;
	UPROPERTY(EditAnywhere)
	float Surface1Z;
	UPROPERTY(EditAnywhere)
	float Surface2Z;
	UPROPERTY(EditAnywhere)
	float WaterSpeed;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Gate;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Crank;
	bool bRisen;

protected:
	bool bNewLevel;

public:
	virtual void Tick(float DeltaTime) override;
	void TurnCrank(int polarity);

protected:
	virtual void BeginPlay() override;
	
	
};
