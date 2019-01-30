// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsConstraintTest.generated.h"

UCLASS()
class WANDERFUL_API APhysicsConstraintTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicsConstraintTest();
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* connectorcomp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* attachmesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
