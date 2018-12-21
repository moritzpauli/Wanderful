// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewBench.h"
#include "FPSwanderfulCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GenericPlatform/GenericPlatformMath.h"



AViewBench::AViewBench() {
	PlayerPos = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("camerapos"));
	bSat = false;
	bGotup = true;
}

void AViewBench::BeginPlay()
{
	Super::BeginPlay();
	PlayerCamera = MyPlayer->Camera;
}

void AViewBench::SitDown()
{
	MyPlayer->bFreeView = false;
	if (MyPlayer->Inventory) {
		MyPlayer->Inventory->SetActorHiddenInGame(true);
	}
	MyPlayer->SetActorEnableCollision(false);
	PlayerOGPosition = MyPlayer->GetActorLocation();
	PlayerOGRotation = MyPlayer->GetActorRotation();

	lerpin = true;
	bSat = true;
	bGotup = false;
}


void AViewBench::GetUp()
{
	lerpin = false;
	lerpout = true;
	

}

void AViewBench::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (lerpin) {
		MyPlayer->SetActorLocation(FMath::Lerp(MyPlayer->GetActorLocation(),PlayerPos->GetComponentLocation(), 0.03f));
		MyPlayer->GetController()->SetControlRotation(FMath::Lerp(MyPlayer->GetActorRotation(), PlayerPos->GetComponentRotation(), 0.04f));
	}

	if (lerpout) {
		MyPlayer->SetActorLocation(FMath::Lerp(MyPlayer->GetActorLocation(),PlayerOGPosition, 0.03f));
		MyPlayer->GetController()->SetControlRotation(FMath::Lerp(MyPlayer->GetActorRotation(), PlayerOGRotation, 0.04f));
		if (FIntVector((FMath::RoundToInt(MyPlayer->GetActorLocation().X)), (FMath::RoundToInt(MyPlayer->GetActorLocation().Y)),  (FMath::RoundToInt(MyPlayer->GetActorLocation().Z)   )) == 
			FIntVector((FMath::RoundToInt(PlayerOGPosition.X)), (FMath::RoundToInt(PlayerOGPosition.Y)), (FMath::RoundToInt(PlayerOGPosition.Z)))
			&& FIntVector((FMath::RoundToInt(MyPlayer->GetActorRotation().Pitch)), (FMath::RoundToInt(MyPlayer->GetActorRotation().Yaw)), (FMath::RoundToInt(MyPlayer->GetActorRotation().Roll))) == 
			FIntVector((FMath::RoundToInt(PlayerOGRotation.Pitch)), (FMath::RoundToInt(PlayerOGRotation.Yaw)), (FMath::RoundToInt(PlayerOGRotation.Roll)))) {
			
			
			
			
			
			MyPlayer->bFreeView = true;
			if (MyPlayer->Inventory) {
				MyPlayer->Inventory->SetActorHiddenInGame(false);
			}
			MyPlayer->SetActorEnableCollision(true);

			bSat = false;
			bGotup = true;
			lerpout = false;
		}
	}

	if (GetBInteract() && !bSat) {
		SitDown();
	}
	if (!GetBInteract() && !bGotup) {
		GetUp();
	}
	
}
