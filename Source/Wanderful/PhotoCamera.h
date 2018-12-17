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
	UPROPERTY(EditAnywhere)
	bool bCameraMode;
	UPROPERTY(EditAnywhere)
	struct FPostProcessSettings PhotoPostProcessing;
	int filmstock;
	UPROPERTY(EditAnywhere)
	class UUserWidget* CameraOverlay;
	UPROPERTY(EditAnywhere)
	FWeightedBlendables MyBlurArray;
	UPROPERTY(EditAnywhere)
	FWeightedBlendable MyBlur;
	

protected:
	FString Testpath;
	FString PathUpictures;
	int PictureCounter;
	class AFPSwanderfulCharacter* MyPlayer;
	TArray<AActor*> MyPlayers;
	FPostProcessSettings OgPostProcessing;
	class UCameraComponent* PlayerCam;
	class UGameSaves* SaveGameInstance;
	bool bFocusBlur;
	bool bFocusComplete;
	bool bShotTaken;
	bool bInFocus;
	bool bShutterClosed;
	float shutterspeed;
	float focusspeed;
	class UPostProcessComponent* PlayerPP;
	FText test;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void TakePhoto();
	void Exposure();
	void EnterCameraMode();
	void ExitCameraMode();
	void DeleteSave();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetupPhotoInputComponent(UInputComponent* PhotoInputComponent);
	void AddFilmRoll();
	
	
};
