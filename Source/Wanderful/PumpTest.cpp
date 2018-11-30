// Fill out your copyright notice in the Description page of Project Settings.

#include "PumpTest.h"
#include "Engine/World.h"

//SetsDefaultValues
APumpTest::APumpTest() {
	PumpHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PumpHandle"));
	PumpHandleComp = CreateDefaultSubobject<USceneComponent>(TEXT("PumpHandleComp"));
	bGetMouse = false;
	PumpHandle->SetupAttachment(PumpHandleComp);
}


void APumpTest::BeginPlay()
{
	Super::BeginPlay();
	

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
		GetWorld()->GetFirstPlayerController()->GetMousePosition(MouseX,MouseY);
	}
	GetWorld()->GetFirstPlayerController()->GetMousePosition(cMouseX,cMouseY);
	
}
