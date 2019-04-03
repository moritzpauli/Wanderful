// Fill out your copyright notice in the Description page of Project Settings.

#include "FloodGate.h"
#include "Components/StaticMeshComponent.h"

AFloodGate::AFloodGate() {
	Gate = CreateDefaultSubobject<UStaticMeshComponent>("FloodGate");
	Crank = CreateDefaultSubobject<UStaticMeshComponent>("Crank");
	bRisen = false;
	bNewLevel = false;
	bDeletedCollider = false;
}




void AFloodGate::BeginPlay()
{
	Super::BeginPlay();
	WaterSurface1->SetActorLocation(FVector(WaterSurface1->GetActorLocation().X, WaterSurface1->GetActorLocation().Y,Surface1ZOG));
	WaterSurface2->SetActorLocation(FVector(WaterSurface2->GetActorLocation().X, WaterSurface2->GetActorLocation().Y, Surface2ZOG));

}

void AFloodGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bRisen && !bNewLevel) {
		WaterSurface1->AddActorWorldOffset(FVector(0,0,-WaterSpeed*DeltaTime));
		WaterSurface2->AddActorWorldOffset(FVector(0, 0, -WaterSpeed * DeltaTime));
		if (WaterSurface1->GetActorLocation().Z <= Surface1Z) {
			bNewLevel = true;
			//UE_LOG(LogTemp, Warning, TEXT("Water lowered"));
			if (!bDeletedCollider) {
				LakeCollision->Destroy();
				bDeletedCollider = true;
			}
		}
	}

}

void AFloodGate::TurnCrank(int polarity)
{
	if (!bRisen) {
		if (Gate->RelativeLocation.Z > -218.0f || polarity==1) {
			Crank->AddLocalRotation(FRotator(0,3*polarity,0));
			Gate->AddLocalOffset(FVector(0, 0, 1 * polarity));
			//UE_LOG(LogTemp, Warning, TEXT("%f"),Crank->RelativeRotation.Pitch);
		}
		if (Gate->RelativeLocation.Z >= 92.0f) {
			bRisen = true;
		}
	}
}
