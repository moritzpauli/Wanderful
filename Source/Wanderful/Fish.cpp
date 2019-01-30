// Fill out your copyright notice in the Description page of Project Settings.

#include "Fish.h"
#include "Components/StaticMeshComponent.h"
#include "FishingSpot.h"
#include "CableComponent.h"


// Sets default values
AFish::AFish()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	MouthSpot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mouthspot"));
	RootMesh->OnComponentEndOverlap.AddDynamic(this, &AFish::OnOverlapEnd);
	bIdle = false;
	bCheckSloth = false;
	RootMesh->SetCollisionProfileName("QueryOnly");
	bMove = true;
	bDestroyable = true;
}

// Called when the game starts or when spawned
void AFish::BeginPlay()
{
	Super::BeginPlay();
	idletimer = idletime;
	movetimer = movetime;
	bSetRotation = false;
	SetActorHiddenInGame(true);
}

// Called every frame
void AFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bSetRotation) {
		RootMesh->SetRelativeRotation(FRotator(0, FMath::RandRange(0.0f, 360.0f), 0));
		bSetRotation = true;
	}

	if (movetimer >= 0.0f && bMove) {
		idletimer = idletime;
		movetimer -= DeltaTime;
		RootMesh->AddWorldOffset(RootMesh->GetForwardVector()*speed*DeltaTime);
		bCheckSloth = false;
	}

	if (movetimer <= 0.0f)
		if (!bCheckSloth) {
			if (FMath::RandRange(1, 10) < sloth) {
				bIdle = true;
				bCheckSloth = true;
			}
			else {
				movetimer = movetime;
				bCheckSloth = true;
			}
		}
	if (bIdle) {
		if (idletimer >= 0.0f) {
			idletimer -= DeltaTime;
		}
		if (idletimer <= 0.0f) {
			movetimer = movetime;
			RootMesh->AddRelativeRotation(FRotator(0, FMath::RandRange(-30.0f, 30.0f), 0));
			bIdle = false;
		}
	}
}



void AFish::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && this != OtherActor && OtherComp) {
		//UE_LOG(LogTemp, Warning, TEXT("U left something"));
		if (OtherActor->GetClass()->IsChildOf(AFishingSpot::StaticClass())) {
			if (OtherComp->GetName() == TEXT("MyFishtank")) {
				//UE_LOG(LogTemp, Warning, TEXT("U left the fishtank!!"));
				RootMesh->AddLocalRotation(FRotator(0.0f, (RootMesh->RelativeRotation.Yaw * 2), 0.0f));
			}

		}
		if (OtherComp->GetName() == TEXT("RespawnTank")) {
			//UE_LOG(LogTemp, Warning, TEXT("RespawnFish"));
			
			if (bDestroyable) {
				Cast<AFishingSpot>(OtherActor)->SpawnFish(Fishdex);
				//UE_LOG(LogTemp, Warning, TEXT("Fish destroyed"));
				Destroy(this);
			}
		}
	}
}

