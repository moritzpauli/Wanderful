// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fish.generated.h"

UCLASS()
class WANDERFUL_API AFish : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFish();
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* RootMesh;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MouthSpot;
	UPROPERTY(EditAnywhere)
	class UPhysicsConstraintComponent* MouthHook;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* HookSpot;
	UPROPERTY(EditAnywhere)
	FString Rarity;
	UPROPERTY(EditAnywhere)
	float speed;
	UPROPERTY(EditAnywhere)
	int sloth;
	UPROPERTY(EditAnywhere)
	float movetime;
	UPROPERTY(EditAnywhere)
	float idletime;
	UPROPERTY(EditAnywhere)
	float HookingProb;
	UPROPERTY(EditAnywhere)
	int Toughness;
	UPROPERTY(EditAnywhere)
	float Strength;
	UPROPERTY(EditAnywhere)
	AActor* HookPivotTemplate;
	bool bIdle;
	int Fishdex;
	bool bMove;
	bool bDestroyable;
	AActor* HookPivot;
	


protected:
	float movetimer;
	float idletimer;
	bool bCheckSloth;
	bool bSetRotation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OnFishHooked();
	void OnFishReeledIn();
	
};
