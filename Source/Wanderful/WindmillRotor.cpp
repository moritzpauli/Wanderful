// Fill out your copyright notice in the Description page of Project Settings.

#include "WindmillRotor.h"


// Sets default values
AWindmillRotor::AWindmillRotor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (WITH_EDITOR)
	{
		PrimaryActorTick.bCanEverTick = true;
		PrimaryActorTick.bStartWithTickEnabled = true;
	}
	RootComp = CreateDefaultSubobject<UStaticMeshComponent>("RootComp");
}

// Called when the game starts or when spawned
void AWindmillRotor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWindmillRotor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bRotate) {
		AddActorLocalRotation(FRotator(0,0,RotSpeed*DeltaTime));
	}

}

bool AWindmillRotor::ShouldTickIfViewportsOnly() const
{
	return true;
}

