// Fill out your copyright notice in the Description page of Project Settings.

#include "PumpTest.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"


//SetsDefaultValues
APumpTest::APumpTest() {
	PumpHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PumpHandle"));
	PumpHandleComp = CreateDefaultSubobject<USceneComponent>(TEXT("PumpHandleComp"));
	WaterfStream = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PumpWater"));
	bGetMouse = false;
	PumpHandle->SetupAttachment(PumpHandleComp);
	
}


void APumpTest::BeginPlay()
{
	Super::BeginPlay();
	midRot = (rotMax + rotMin) / 2;
	PumpHandleComp->SetRelativeRotation(FRotator(midRot,0,0));
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
		StartCount = FMath::RandRange(6, 12);
		Counter = 0;
	}
	GetWorld()->GetFirstPlayerController()->GetMousePosition(cMouseX,cMouseY);
	PumpHandleComp->SetRelativeRotation(FRotator(cMouseY/(598/(rotMin-rotMax)),0,0));
	//UE_LOG(LogTemp, Warning, TEXT("%f"), PumpHandleComp->RelativeRotation.Pitch);
	if (PumpHandleComp->RelativeRotation.Pitch <= rotMin-359.8f) {
		bHitEnd = true;
	}
	if (PumpHandleComp->RelativeRotation.Pitch >= rotMax-360.2f) {
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
