// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FilmRoll.generated.h"

UCLASS()
class WANDERFUL_API AFilmRoll : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFilmRoll();
	bool bPickedUp;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MyMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	class APhotoCamera* MyPCam;
	TArray<AActor*> MyCams;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnPickUp();
	
	
};
