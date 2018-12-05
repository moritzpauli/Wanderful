// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticInteractable.h"
#include "Components/BoxComponent.h"
#include "FPSwanderfulCharacter.h"
#include "Engine.h"
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
	bInteracting = false;
	bInView = false;
	bInRange = false;
	bInteractEndReadyOne = false;
	bInteractEndReadyTwo = false;
	bInteractPressed = false;
}

// Called when the game starts or when spawned
void AStaticInteractable::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPSwanderfulCharacter::StaticClass(), MyPlayers);
	MyPlayer = Cast<AFPSwanderfulCharacter>(MyPlayers[0]);
	
}

// Called every frame
void AStaticInteractable::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	bInteractPressed = MyPlayer->GetInteractPressed();
	if (bInRange && bInView && !bInteracting) {
		if (bInteractPressed) {
			OnInteract();
		}
	}
	if (bInteracting && !bInteractPressed) {
		bInteractEndReadyOne = true;
	}
	if (bInteractEndReadyOne && bInteractPressed) {
		bInteractEndReadyTwo = true;
	}
	if (bInteractEndReadyTwo && !bInteractPressed) {
		OnInteractEnd();
	}
	
}


void AStaticInteractable::OnInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Static Interact Time "));
	bInteracting = true;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPSwanderfulCharacter::StaticClass(), MyPlayers);
	Cast<AFPSwanderfulCharacter>(MyPlayers[0])->bCanMove = false;
	Cast<AFPSwanderfulCharacter>(MyPlayers[0])->bFreeView = false;
	
}

void AStaticInteractable::OnInteractEnd() {
	bInteracting = false;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPSwanderfulCharacter::StaticClass(), MyPlayers);
	Cast<AFPSwanderfulCharacter>(MyPlayers[0])->bCanMove = true;
	Cast<AFPSwanderfulCharacter>(MyPlayers[0])->bFreeView = true;
	bInteractEndReadyOne = false;
	bInteractEndReadyTwo = false;
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


