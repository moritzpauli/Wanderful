// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUp.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APickUp::APickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh"));
	mesh->SetSimulatePhysics(true);
	RootComponent = mesh;
	gravity = true;
	pickedUP = false;
}

void APickUp::EnablePhysics()
{
	mesh->SetSimulatePhysics(true);
	UE_LOG(LogTemp, Warning, TEXT("Physics Reactivated"));
}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
	pickedUP = false;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AFPSwanderfulCharacter::StaticClass() ,MyPlayers);
	MyPlayer = Cast<AFPSwanderfulCharacter>(MyPlayers[0]);
	PlayerCamera = MyPlayer->FindComponentByClass<UCameraComponent>();

	TArray<USceneComponent*> Components;

	MyPlayer->GetComponents(Components);

	if (Components.Num() > 0) {
		for (auto& Comp : Components) {
			if (Comp->GetName() == "HoldingComponent") {
				HoldingComp = Cast<USceneComponent>(Comp);
			}
		}
	}
}

// Called every frame
void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//picking up the stick
	if (pickedUP) {

		if (HoldingComp) {
			SetActorLocationAndRotation(HoldingComp->GetComponentLocation(),HoldingComp->GetComponentRotation());
		}

		//LEGACY CODE
		/*mesh->SetSimulatePhysics(false);
		//AttachToComponent();
		GetWorldTimerManager().ClearTimer(physicsReset);
		setTimer = true;*/
	}
	if (!pickedUP) {
		

		//LEGACY CODE
		/*
		if (setTimer) {
			GetWorld()->GetTimerManager().SetTimer(physicsReset, this, &APickUp::EnablePhysics, 0.1f, false);
			UE_LOG(LogTemp, Warning, TEXT("timer set"));
			setTimer = false;
			
		}*/
			
		
		
		
	}

}


void APickUp::InspectActor()
{

	ControlRotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	SetActorRotation(FQuat(ControlRotation));
}

void APickUp::PickUpObject() 
{
	pickedUP = !pickedUP;
	gravity = !gravity;
	mesh->SetEnableGravity(gravity);
	mesh->SetSimulatePhysics(pickedUP ? false : true);
	mesh->SetCollisionEnabled(pickedUP ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

	if (!pickedUP && !inspectable) {
		ForwardVector = PlayerCamera->GetForwardVector();
		mesh->AddForce(ForwardVector * 100000 * mesh->GetMass());
	}
	if (!pickedUP && inspectable) {
		ForwardVector = PlayerCamera->GetForwardVector();
		mesh->AddForce(ForwardVector * 100 * mesh->GetMass());
	}

}

