// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticInteractable.generated.h"


UCLASS()
class WANDERFUL_API AStaticInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaticInteractable();
	UPROPERTY(EditAnywhere)
	bool bInView;
	UPROPERTY(EditAnywhere)
	bool bInRange;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* myMesh;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;
	UPROPERTY(EditAnywhere)	
	bool bInteracting;
	UPROPERTY(EditAnywhere)
	bool bLockView;
	UPROPERTY(EditAnywhere)
	bool bLockMovement;
	TArray<AActor*> MyPlayers;
	class AFPSwanderfulCharacter* MyPlayer;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool bInteractEndReadyOne;
	bool bInteractEndReadyTwo;
	bool bInteractPressed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//bind input
	

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void OnInteract();
	void OnInteractEnd();
	
	bool GetBInteract() { return bInteracting; };
	
};
