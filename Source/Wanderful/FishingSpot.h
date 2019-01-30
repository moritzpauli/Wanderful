// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaticInteractable.h"
#include "FishingSpot.generated.h"

/**
 * 
 */
UCLASS()
class WANDERFUL_API AFishingSpot : public AStaticInteractable
{
	GENERATED_BODY()

public:
	AFishingSpot();

	UPROPERTY(EditAnywhere)
	AActor* TestSphere;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Fishtank;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* RespawnTank;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Float;
	bool bTankInView;
	UPROPERTY(EditAnywhere)
	FVector FloatPosition;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* FishtankPlane;
	UPROPERTY(EditAnywhere)
	class USceneComponent* WireConnector;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* HookTrigger;
	UPROPERTY(EditAnywhere)
	TArray<class AFish*> FishVariants;
	UPROPERTY(EditAnywhere)
	TArray<class AFish*> SpawnedFish;
	/*UPROPERTY(EditAnywhere)
	class UCableComponent* FloatToFish;
	UPROPERTY(EditAnywhere)
	class UPhysicsConstraintComponent* PhysicsFloatToFish;*/
	UPROPERTY(EditAnywhere)
	int FishAmount;
	UPROPERTY(EditAnywhere)
	bool bHooked;
	UPROPERTY(EditAnywhere)
	class AFish* HookedFish;
	UPROPERTY(EditAnywhere)
	float Balance;
	UPROPERTY(EditAnywhere)
	float StruggleTimeMin;
	UPROPERTY(EditAnywhere)
	float StruggleTimeMax;
	UPROPERTY(EditAnywhere)
	float MouseWheelSens;
	UPROPERTY(EditAnywhere)
	float ScaleMax;
	UPROPERTY(EditAnywhere)
	float ReelSpeed;
	UPROPERTY(EditAnywhere)
	float FloatWeight;
	UPROPERTY(EditAnywhere)
	bool CurrentFishDestroyable;
	bool bHookCast;
	bool bOnInteractStart;
	bool bOnInteractEnd;
	bool bCaught;


protected:
	bool SetHeight;
	FTransform PlaneTransform;
	float RunningTime;
	float PullTime;
	bool bSpawnFish;
	FVector TankBounds;
	FTransform TankTransform;
	FTransform FishTransform;
	FVector FishBounds;
	FVector TankBoundsMin;
	FVector TankBoundsMax;
	FVector OgFloatPos;
	int bobcount;
	int strugglecounter;
	float struggletimer;
	float FHeightMax;
	bool countstop;
	bool bFloatpull;
	bool bIdle;
	bool bStruggle;
	bool lerpin;
	bool bGetHookPos;
	FVector OgHookPos;
	float progress;
	FVector FloatStartPosition;

public:
	virtual void Tick(float DeltaTime) override;
	void ToggleCastHook();
	void SpawnFish(int Index);
	void ReceiveReelInput( int polarity);

	UFUNCTION()
	void OnHookOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

protected:
	void BeginPlay() override;
	void FishFree();
	void FishCatch();
	
	
};
