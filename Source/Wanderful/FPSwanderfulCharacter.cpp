// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSwanderfulCharacter.h"
#include "Camera/CameraComponent.h"


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

// Called every frame
void AFPSwanderfulCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSwanderfulCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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


