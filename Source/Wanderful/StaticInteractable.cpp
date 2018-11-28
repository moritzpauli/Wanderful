// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticInteractable.h"
#include "Components/BoxComponent.h"
#include "FPSwanderfulCharacter.h"


// Sets default values
AStaticInteractable::AStaticInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	myMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetCollisionProfileName("Trigger");
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AStaticInteractable::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AStaticInteractable::OnOverlapEnd);
	bInteractReady = false;
	bInView = false;
	bInRange = false;
}

// Called when the game starts or when spawned
void AStaticInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStaticInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bInRange && bInView) {
		OnInteractReady();
		bInteractReady = true;
	}

}

void AStaticInteractable::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->GetClass()->IsChildOf(AFPSwanderfulCharacter::StaticClass())) {
		bInRange = true;
	}
	
}

void AStaticInteractable::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->IsChildOf(AFPSwanderfulCharacter::StaticClass())) {
		bInRange = false;
	}
}

void AStaticInteractable::OnInteractReady()
{
	bInteractReady = true;
}

