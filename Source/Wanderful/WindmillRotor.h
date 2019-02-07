// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindmillRotor.generated.h"

UCLASS()
class WANDERFUL_API AWindmillRotor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWindmillRotor();
	UPROPERTY(EditAnywhere)
	float RotSpeed;
	UPROPERTY(EditAnywhere)
	bool bRotate;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* RootComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override;

	
	
};
