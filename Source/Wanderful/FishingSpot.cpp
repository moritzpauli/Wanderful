// Fill out your copyright notice in the Description page of Project Settings.

#include "FishingSpot.h"
#include "Components/StaticMeshComponent.h"

AFishingSpot::AFishingSpot() {
	Fishtank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyFishtank"));
	Float = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyFloat"));
	FishtankPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyFishtankPlane"));
	WireConnector = CreateDefaultSubobject<USceneComponent>(TEXT("WireConnector"));
	WireConnector->SetupAttachment(Float);
}


void AFishingSpot::BeginPlay()
{
	Super::BeginPlay();
	FishtankPlane->SetWorldTransform(Fishtank->GetComponentTransform());
	FishtankPlane->SetWorldScale3D(FVector(FishtankPlane->GetComponentScale().X, FishtankPlane->GetComponentScale().Y,0.01f));
	FishtankPlane->AddLocalOffset(FVector(0.0f, 0.0f, (Fishtank->CalcBounds(PlaneTransform).BoxExtent.Z))*Fishtank->RelativeScale3D.Z);
	myMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}



void AFishingSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetBInteract()) {
		myMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FishtankPlane->SetVisibility(true);
		if (bTankInView) {
			Float->SetWorldLocation(FloatPosition);
		}
	}
	if (!GetBInteract()) {
		myMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		FishtankPlane->SetVisibility(false);
	}

}

