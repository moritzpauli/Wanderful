// Fill out your copyright notice in the Description page of Project Settings.

#include "PumpTest.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"


//SetsDefaultValues
APumpTest::APumpTest() {
	PumpHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PumpHandle"));
	PumpHandleComp = CreateDefaultSubobject<USceneComponent>(TEXT("PumpHandleComp"));
	WaterfStream = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PumpWater"));
	SteelRod = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rod"));
	bGetMouse = false;
	PumpHandle->SetupAttachment(PumpHandleComp);
	RodZmin = -18.0f;
	RodZmax = 0.0f;
	
}


void APumpTest::BeginPlay()
{
	Super::BeginPlay();
	midRot = (rotMax + rotMin) / 2;
	PumpHandleComp->SetRelativeRotation(FRotator(0,0,0));
	if(WaterfStream)
	WaterfStream->Deactivate();

}

void APumpTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PumpInteracting = GetBInteract();
	if (PumpInteracting) {
		PumpAction();
	}
	if (!PumpInteracting) {
		bGetMouse = false;
	}

}

void APumpTest::PumpAction()
{
	if (!bGetMouse) {
		bGetMouse = true;
		StartCount = FMath::RandRange(15, 25);
		Counter = 0;
	}
	GetWorld()->GetFirstPlayerController()->GetMousePosition(cMouseX,cMouseY);
	PumpHandleComp->SetRelativeRotation(FRotator(0,0,cMouseY/(1079/(rotMin-rotMax))));
	SteelRod->SetRelativeLocation(FVector(0, 0, cMouseY /(1079/(RodZmin-RodZmax))));
	//UE_LOG(LogTemp, Warning, TEXT("%f"), PumpHandleComp->RelativeRotation.Roll);
	UE_LOG(LogTemp, Warning, TEXT("%f"), cMouseY);
	if (PumpHandleComp->RelativeRotation.Roll >= rotMin) {
		bHitEnd = true;
	}
	if (PumpHandleComp->RelativeRotation.Roll <= rotMax) {
		bHitStart = true;
		
	}
	if (bHitStart && bHitEnd) {
		Counter++;
		bHitStart = false;
		bHitEnd = false;
		//UE_LOG(LogTemp, Warning, TEXT("%d"), Counter);
	}
	if (Counter >= StartCount) {
		UE_LOG(LogTemp, Warning, TEXT("Wasser arsch"));
		if(WaterfStream)
		WaterfStream->Activate();
	}
	TestRotator = PumpHandleComp->RelativeRotation;

}
