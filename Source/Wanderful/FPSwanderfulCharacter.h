// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"
#include "StaticInteractable.h"
#include "Components/AudioComponent.h"
#include "FPSwanderfulCharacter.generated.h"


UCLASS()
class WANDERFUL_API AFPSwanderfulCharacter : public ACharacter, public IGameplayTagAssetInterface
{
	GENERATED_BODY()






public:
	// Sets default values for this character's properties
	AFPSwanderfulCharacter();
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer GameplayTags;
	UPROPERTY(EditAnywhere)
	AActor* Inventory;	
	UPROPERTY(EditAnywhere)
	class UPostProcessComponent* InspectBlur;
	UPROPERTY(EditAnywhere)
	class UPostProcessComponent* FocusBlur;
	UPROPERTY(EditAnywhere)
	class UPhysicsConstraintComponent* RodTip;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* LineStartComponent;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* HoldCamera;
	UPROPERTY(EditAnywhere)
	TArray<class AFish*> MyCatches;
	UPROPERTY(EditAnywhere)
	bool bPuzzlePu;
	UPROPERTY(EditAnywhere)
	bool bFire;
	bool InteractPressed;
	UPROPERTY(EditAnywhere)
	bool bFreeView;
	bool bPhotoCamera;
	UPROPERTY(EditAnywhere)
	AActor* CurrentInView;
	UPROPERTY(EditAnywhere)
	AActor* FishingTestActor;
	UPROPERTY(EditAnywhere)
	class USceneComponent* LineStartPosition;
	UPROPERTY(EditAnywhere, Category = Audio, meta = (AllowPrivateAcess = "true"))
	class USoundCue* FootstepSoundCue;
	UPROPERTY(EditAnywhere, Category = Audio, meta = (AllowPrivateAcess = "true"))
	class USoundCue* FootstepSoundCueForest;
	float RayCastLength;
	bool bCameraInHand;
	bool bCountedCatchI;
	UPROPERTY(EditAnywhere)
	float FootstepCadence;
	bool bMovingF;
	bool bMovingR;
	UPROPERTY(EditAnywhere)
	UPhysicalMaterial* ForestMat;
	UPROPERTY(EditAnywhere)
	UPhysicalMaterial* TestMat;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool CastRay(FHitResult &HitResult);
	void Interact();
	void OnInspect();
	void OnInspectReleased();
	void OnFiring();
	void OnWheelUp();
	void OnWheelDown();
	void OnFishingTest();
	void OnTakeCamera();
	void FishingComplete();
	void PlayFootstepSound();


protected:
	bool bBacklash;
	bool bFilmRoll;
	float StartWhackRotation;
	bool bBobStrafe;
	bool bBobStraight;
	FHitResult hit;
	bool bFishingTest;
	FRotator ControlRotation;
	FVector CameraHoldingPosition;
	class APhotoCamera* LevelPhotoCamera;
	TArray<AActor*> LevelPhotoCameras;
	bool ConstraintInitialized;
	FVector LineStartOriginal;
	int CatchIndex;
	bool bFishInspect;
	float OgCableLength;
	UAudioComponent* FootstepAudioComponent;
	UAudioComponent* FootstepAudioComponentForest;
	float FootstepTimer;
	FTransform CharTransform;
	


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = GameplayTags; return; }


	UPROPERTY(EditAnywhere)
	class USceneComponent* HoldingComponent;
	UPROPERTY(EditAnywhere)
	class USceneComponent* StickHoldingComponent;
	UPROPERTY(EditAnywhere)
	class USceneComponent* FishInspectComponent;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* FishingRod;
	UPROPERTY(EditAnywhere)
	class UCableComponent* FishingWire;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> MyShake;
	UPROPERTY(EditAnywhere)
	UCameraShake* MyShakeUC;
	UPROPERTY(EditAnywhere)
	class APickUp* CurrentItem;

	UPROPERTY(EditAnywhere)
	FVector HoldingPosition;
	UPROPERTY(EditAnywhere)
	FTransform StickHoldingTransform;
	UPROPERTY(EditAnywhere)
	float whackspeed;
	UPROPERTY(EditAnywhere)
	float whacklimit;
	UPROPERTY(EditAnywhere)
	float MouseSensX;
	UPROPERTY(EditAnywhere)
	float MouseSensY;

	bool bCanMove;
	bool bHoldingPickUp;
	UPROPERTY(EditAnywhere)
	bool bInspecting;
	UPROPERTY(EditAnywhere)
	bool bFishExitInspect;
	UPROPERTY(EditAnywhere)
	bool bFishEnterInspect;

	float rotationMax;
	float rotationMin;

	FVector HoldingComp;
	FRotator LastRotation;


	UFUNCTION()
		void MoveForward(float value);

	UFUNCTION()
		void MoveRight(float value);

	UFUNCTION()
		void StartJump();

	UFUNCTION()
		void StopJump();


	void ToggleMovement();
	void ToggleItemPU();
	void FishingRodOn();
	void FishingRodOff();
	void InteractEnd() { InteractPressed = false; };
	bool GetInteractPressed() { return InteractPressed; };
	
};
