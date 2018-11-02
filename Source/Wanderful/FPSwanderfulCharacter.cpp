// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSwanderfulCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine.h"
#include "Pickup.h"






// Sets default values
AFPSwanderfulCharacter::AFPSwanderfulCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
}

// Called when the game starts or when spawned
void AFPSwanderfulCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine) {
		//put up a debug message for 5 sec. the -1 "key" value (first argument) indicates theat we will never have to update/refresh this message
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Using this FPS character!"));
	}
}


//raycast
bool AFPSwanderfulCharacter::CastRay(FHitResult  &HitResult)
{
	
	//sartpoint
	FVector StartTrace = Camera->GetComponentLocation();
	//Direction
	FVector ForwardVector = Camera->GetForwardVector();
	//length
	FVector EndTrace = ((ForwardVector*1000.f) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Magenta, true);
	return GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams);
		
		
	
}

void AFPSwanderfulCharacter::Interact()
{
	FHitResult hit;

	if (CastRay(hit)) {
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, hit.GetActor()->GetName());
		if (hit.GetActor()->GetClass()->GetName()==("BP_Pickup_C")) {
			//hit.GetActor()->Destroy();
			//TODO Pickup Actor
			hit.GetActor()->GetClass()->GetDefaultObject<APickUp>()->pickedUP = true;
			Inventory = hit.GetActor();
			UE_LOG(LogTemp, Warning, TEXT("hitit"));
		}
	}
	

	/*if (CastRay() != nullptr) {
		if (CastRay()->GetActor()->GetName() == "PickUp1") {
			CastRay()->GetActor()->Destroy();
		}
	}*/
	//UE_LOG(LogTemp, Warning, TEXT("Interacted"));
}

// Called every frame
void AFPSwanderfulCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AFPSwanderfulCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFPSwanderfulCharacter::Interact);
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSwanderfulCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSwanderfulCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AFPSwanderfulCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSwanderfulCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPSwanderfulCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPSwanderfulCharacter::AddControllerPitchInput);

	//jumping
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSwanderfulCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSwanderfulCharacter::StopJump);

}





void AFPSwanderfulCharacter::MoveForward(float value)
{
	//find out forward direction
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, value);
}

void AFPSwanderfulCharacter::MoveRight(float value)
{
	//find out right direction
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value);
}

void AFPSwanderfulCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSwanderfulCharacter::StopJump()
{
	bPressedJump = false;
}




