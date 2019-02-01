// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "FPSwanderfulCharacter.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"

#include "PickUp.generated.h"


UCLASS()
class WANDERFUL_API APickUp : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUp();
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh;
	UPROPERTY(EditAnywhere)
	USceneComponent* HoldingComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
	FGameplayTagContainer GameplayTags;
	bool pickedUP;
	UPROPERTY(EditAnywhere)
	FString msg;
	UPROPERTY(EditAnywhere)
	AActor* PlayerActor;
	bool setTimer;
	bool gravity;
	bool bInspecting;
	bool bPuzzlePlaced;

	UPROPERTY(EditAnywhere)
	bool throwable;
	UPROPERTY(EditAnywhere)
	bool wieldable;
	UPROPERTY(EditAnywhere)
	bool inspectable;
	UPROPERTY(EditAnywhere)
	bool puzzleitem;
	UPROPERTY(EditAnywhere)
	bool puzzlepu;
	UPROPERTY(EditAnywhere)
	bool stackable;
	

	UFUNCTION()
	void InspectActor();

	UFUNCTION()
	void PickUpObject();
	void PickUpObjectLight();
	void ThrowObjectLight();

	FRotator ControlRotation;
	AFPSwanderfulCharacter* MyPlayer;
	UCameraComponent* PlayerCamera;
	FVector ForwardVector;
	
protected:
	FTimerHandle physicsReset;
	
	TArray<AActor*> MyPlayers;
	void EnablePhysics();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = GameplayTags; return; }
	void SetPu(bool pickupNew) { pickedUP = pickupNew; UE_LOG(LogTemp,Warning,TEXT("setPU"))};
	bool GetPu() { return pickedUP; };
	
	
};
