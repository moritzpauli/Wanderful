// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaticInteractable.h"
#include "ViewBench.generated.h"

/**
 * 
 */
UCLASS()
class WANDERFUL_API AViewBench : public AStaticInteractable
{
	GENERATED_BODY()
	
public:
	AViewBench();
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PlayerPos;
	UPROPERTY(EditAnywhere)
	float LookMaxY;
	UPROPERTY(EditAnywhere)
	float LookMaxX;
	UPROPERTY(EditAnywhere)
	FVector2D LookLimits;
	UPROPERTY(EditAnywhere)
	bool lerpin;
	UPROPERTY(EditAnywhere)
	FRotator OgPlayerRot;




private:
	class UCameraComponent* PlayerCamera;
	FTransform CameraOGTransform;
	bool bSat;
	bool bGotup;
	bool lerpout;
	FVector PlayerOGPosition;
	FRotator PlayerOGRotation;
	FRotator TempMinRot;
	FRotator TempMaxRot;
	

private:
	virtual void BeginPlay() override;
	void SitDown();
	void GetUp();

public:
	virtual void Tick(float DeltaTime) override;
	
};
