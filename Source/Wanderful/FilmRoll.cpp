// Fill out your copyright notice in the Description page of Project Settings.

#include "FilmRoll.h"
#include "Kismet/GameplayStatics.h"
#include "PhotoCamera.h"
#include "Engine/World.h"



// Sets default values
AFilmRoll::AFilmRoll()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bPickedUp = false;
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void AFilmRoll::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APhotoCamera::StaticClass(), MyCams);
	MyPCam = Cast<APhotoCamera>(MyCams[0]);
}

// Called every frame
void AFilmRoll::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFilmRoll::OnPickUp()
{
	bPickedUp = true;
	MyPCam->AddFilmRoll();
	Destroy();
}

