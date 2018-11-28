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
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
};
