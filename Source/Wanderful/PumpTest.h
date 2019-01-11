// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaticInteractable.h"
#include "PumpTest.generated.h"

/**
 * 
 */
UCLASS()
class WANDERFUL_API APumpTest : public AStaticInteractable
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APumpTest();
	UPROPERTY(EditAnywhere)
	bool PumpInteracting;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PumpHandle;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SteelRod;
	UPROPERTY(EditAnywhere)
	USceneComponent* PumpHandleComp;
	UPROPERTY(EditAnywhere)
	float rotMax;
	UPROPERTY(EditAnywhere)
	float rotMin;
	UPROPERTY(EditAnywhere)
	float rotStep;
	UPROPERTY(EditAnywhere)
	float cMouseX;
	UPROPERTY(EditAnywhere)
	float cMouseY;
	UPROPERTY(EditAnywhere)
	FRotator TestRotator;
	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* WaterfStream;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool bGetMouse;
	float MouseX;
	float MouseY;
	float midRot;
	int StartCount;
	int Counter;
	bool bHitMiddle;
	bool bHitEnd;
	bool bHitStart;
	float RodZmin;
	float RodZmax;
	float MouseMax;
	FVector2D ViewportSize;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void PumpAction();
	
};
