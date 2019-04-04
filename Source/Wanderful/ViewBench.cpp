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
	LookMaxX = 40.0f;
	LookMaxY = 30.0f;
}

void AViewBench::BeginPlay()
{
	Super::BeginPlay();
	PlayerCamera = MyPlayer->Camera;
	
}

void AViewBench::SitDown()
{
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
		MyPlayer->SetActorLocation(FMath::Lerp(MyPlayer->GetActorLocation(), PlayerPos->GetComponentLocation(), 1.5f*DeltaTime));
		MyPlayer->GetController()->SetControlRotation(FMath::Lerp(MyPlayer->GetActorRotation(), PlayerPos->GetComponentRotation(), 1.8f*DeltaTime));
		MyPlayer->bFreeView = false;
		if (FIntVector((FMath::RoundToInt(MyPlayer->GetActorLocation().X)), (FMath::RoundToInt(MyPlayer->GetActorLocation().Y)), (FMath::RoundToInt(MyPlayer->GetActorLocation().Z))) ==
			FIntVector((FMath::RoundToInt(PlayerPos->GetComponentLocation().X)), (FMath::RoundToInt(PlayerPos->GetComponentLocation().Y)), (FMath::RoundToInt(PlayerPos->GetComponentLocation().Z)))
			&& FIntVector((FMath::RoundToInt(MyPlayer->GetActorRotation().Pitch)), (FMath::RoundToInt(MyPlayer->GetActorRotation().Yaw)), (FMath::RoundToInt(MyPlayer->GetActorRotation().Roll))) ==
			FIntVector((FMath::RoundToInt(PlayerPos->GetComponentRotation().Pitch)), (FMath::RoundToInt(PlayerPos->GetComponentRotation().Yaw)), (FMath::RoundToInt(PlayerPos->GetComponentRotation().Roll)))) {
			MyPlayer->bFreeView = true;
			OgPlayerRot = MyPlayer->GetActorRotation().GetDenormalized();
			lerpin = false;
			
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("%f"), MyPlayer->GetActorRotation().GetDenormalized().Yaw);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), OgPlayerRot.Yaw);
	if (!lerpin && !lerpout && bSat) {
		TempMaxRot = FRotator(0.0f,OgPlayerRot.Yaw , 0.0f);
		TempMinRot = FRotator(0.0f, OgPlayerRot.Yaw, 0.0f);
		if (MyPlayer->GetActorRotation().GetDenormalized().Yaw > OgPlayerRot.Yaw + LookLimits.X) {
			MyPlayer->GetController()->SetControlRotation(FRotator(MyPlayer->GetController()->GetControlRotation().Pitch, PlayerPos->GetComponentRotation().Yaw + LookLimits.X, MyPlayer->GetController()->GetControlRotation().Roll));
			//UE_LOG(LogTemp, Warning, TEXT("X"));

		}
		if (MyPlayer->GetActorRotation().GetDenormalized().Yaw < OgPlayerRot.Yaw - LookLimits.X) {
			MyPlayer->GetController()->SetControlRotation(FRotator(MyPlayer->GetController()->GetControlRotation().Pitch, PlayerPos->GetComponentRotation().Yaw - LookLimits.X, MyPlayer->GetController()->GetControlRotation().Roll));
			//UE_LOG(LogTemp, Warning, TEXT("Y"));
		}
		if (MyPlayer->GetController()->GetControlRotation().Pitch > PlayerPos->GetComponentRotation().Pitch + LookLimits.Y && MyPlayer->GetController()->GetControlRotation().Pitch < 180.0f) {
			MyPlayer->GetController()->SetControlRotation(FRotator(PlayerPos->GetComponentRotation().Pitch + LookLimits.Y, MyPlayer->GetController()->GetControlRotation().Yaw, MyPlayer->GetController()->GetControlRotation().Roll));
			//FString lookangle = FString::SanitizeFloat(MyPlayer->GetController()->GetControlRotation().Pitch);
			//UE_LOG(LogTemp, Warning, TEXT("X"));

		}
		if (MyPlayer->GetController()->GetControlRotation().Pitch < PlayerPos->GetComponentRotation().Pitch - LookLimits.Y + 360.0f && MyPlayer->GetController()->GetControlRotation().Pitch>180.0f) {
			MyPlayer->GetController()->SetControlRotation(FRotator(PlayerPos->GetComponentRotation().Pitch - LookLimits.Y, MyPlayer->GetController()->GetControlRotation().Yaw, MyPlayer->GetController()->GetControlRotation().Roll));
			//UE_LOG(LogTemp, Warning, TEXT("X"));

		}
	}

	if (lerpout) {
		MyPlayer->bFreeView = false;
		MyPlayer->SetActorLocation(FMath::Lerp(MyPlayer->GetActorLocation(), PlayerOGPosition, 3.0f*DeltaTime));
		//MyPlayer->GetController()->SetControlRotation(FMath::Lerp(MyPlayer->GetActorRotation(), PlayerOGRotation, 8.0f*DeltaTime));
		if (FIntVector((FMath::RoundToInt(MyPlayer->GetActorLocation().X)), (FMath::RoundToInt(MyPlayer->GetActorLocation().Y)), (FMath::RoundToInt(MyPlayer->GetActorLocation().Z))) ==
			FIntVector((FMath::RoundToInt(PlayerOGPosition.X)), (FMath::RoundToInt(PlayerOGPosition.Y)), (FMath::RoundToInt(PlayerOGPosition.Z)))
			/*&& FIntVector((FMath::RoundToInt(MyPlayer->GetActorRotation().Pitch)), (FMath::RoundToInt(MyPlayer->GetActorRotation().Yaw)), (FMath::RoundToInt(MyPlayer->GetActorRotation().Roll))) ==
			FIntVector((FMath::RoundToInt(PlayerOGRotation.Pitch)), (FMath::RoundToInt(PlayerOGRotation.Yaw)), (FMath::RoundToInt(PlayerOGRotation.Roll)))*/) {





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
