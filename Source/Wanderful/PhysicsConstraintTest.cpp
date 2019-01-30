// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsConstraintTest.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
APhysicsConstraintTest::APhysicsConstraintTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	connectorcomp = CreateDefaultSubobject<UStaticMeshComponent>("scenecomp");
	attachmesh = CreateDefaultSubobject<UStaticMeshComponent>("attach");

}

// Called when the game starts or when spawned
void APhysicsConstraintTest::BeginPlay()
{
	Super::BeginPlay();
	RootComponent = connectorcomp;
}

// Called every frame
void APhysicsConstraintTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	attachmesh->AddWorldOffset(FVector(0, 0, 30 * DeltaTime));
	//AddActorWorldOffset(FVector(0, 0, 10 * DeltaTime));

}

