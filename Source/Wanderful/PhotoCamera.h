// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhotoCamera.generated.h"

UCLASS()
class WANDERFUL_API APhotoCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhotoCamera();

protected:
	FString Testpath;
	FString PathUpictures;
	int PictureCounter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void TakePhoto();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetupPhotoInputComponent(UInputComponent* PhotoInputComponent);
	
	
};
