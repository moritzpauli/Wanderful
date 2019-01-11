// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSwanderfulCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine.h"
#include "Pickup.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FilmRoll.h"
#include "PuzzleItemSpot.h"






// Sets default values
AFPSwanderfulCharacter::AFPSwanderfulCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	HoldingComponent = CreateDefaultSubobject<USceneComponent>("HoldingComponent");
	StickHoldingComponent = CreateDefaultSubobject<USceneComponent>("StickHoldingComponent");
	HoldingComponent->SetRelativeLocation(HoldingPosition);
	HoldingComponent->SetupAttachment(Camera);
	StickHoldingComponent->SetRelativeTransform(StickHoldingTransform);
	StickHoldingComponent->SetupAttachment(Camera);
	CurrentItem = NULL;
	bCanMove = true;
	bInspecting = false;
	InspectBlur = CreateDefaultSubobject<UPostProcessComponent>("InspectBlur");
	FocusBlur = CreateDefaultSubobject<UPostProcessComponent>("FocusBlur");
	StartWhackRotation = 69.0f;
	bFire = false;
	MyShakeUC = Cast<UCameraShake>(MyShake);
	bBobStraight = false;
	bBobStrafe = false;
	InteractPressed = false;
	bFreeView = true;
	bPhotoCamera = false;
	bFilmRoll = false;
}

// Called when the game starts or when spawned
void AFPSwanderfulCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine) {
		//put up a debug message for 5 sec. the -1 "key" value (first argument) indicates theat we will never have to update/refresh this message
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Using this FPS character!"));
	}

	rotationMax = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax;
	rotationMin = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin;
	InspectBlur->bEnabled = false;
	GetWorld()->GetFirstPlayerController()->InputPitchScale = MouseSensY * (-1);
	GetWorld()->GetFirstPlayerController()->InputYawScale = MouseSensX;


}


// Called every frame
void AFPSwanderfulCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (Inventory) {
		bPuzzlePu = Cast<APickUp>(Inventory)->puzzlepu;
	}*/

	if (CastRay(hit)) {
		if (hit.GetActor()->GetClass()->IsChildOf(APuzzleItemSpot::StaticClass())) {
			if (Cast<APuzzleItemSpot>(hit.GetActor())->ClueItem == Inventory) {
				if (!bPhotoCamera) {
					CurrentInView = hit.GetActor();
					Cast<APuzzleItemSpot>(hit.GetActor())->bInView = true;
				}
				if (InteractPressed) {
					Cast<APuzzleItemSpot>(hit.GetActor())->SetInPosition();
					//UE_LOG(LogTemp, Warning, TEXT("Inposition"));
					//Cast<APickUp>(Inventory)->bPuzzlePlaced = true;
					if (Inventory) {
						Inventory->Destroy();
					}
					Inventory = NULL;
					CurrentItem = NULL;
					//bHoldingPickUp = false;
				}
			}
		}
		else {
			if (CurrentInView) {
				if (CurrentInView->GetClass()->IsChildOf(APuzzleItemSpot::StaticClass())) {
					Cast<APuzzleItemSpot>(CurrentInView)->bInView = false;
				}
			}
		}
	}
	else
	{
		if (!bHoldingPickUp) {
			CurrentItem = NULL;
			
		}
		if (CurrentInView) {
			if (CurrentInView->GetClass()->IsChildOf(APuzzleItemSpot::StaticClass())) {
				Cast<APuzzleItemSpot>(CurrentInView)->bInView = false;
			}
		}
	}


	if (!bHoldingPickUp) {
		if (CastRay(hit)) {



			if (hit.GetActor()->GetClass()->IsChildOf(AFilmRoll::StaticClass()) && !bPhotoCamera) {
				bFilmRoll = true;
			}
			else {
				bFilmRoll = false;
			}

			if (hit.GetActor()->GetClass()->IsChildOf(AStaticInteractable::StaticClass())) {
				CurrentInView = hit.GetActor();
				Cast<AStaticInteractable>(CurrentInView)->bInView = true;
			}
			else {
				if (CurrentInView) {
					if (CurrentInView->GetClass()->IsChildOf(AStaticInteractable::StaticClass())) {
						Cast<AStaticInteractable>(CurrentInView)->bInView = false;
					}
				}
			}

			GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, hit.GetActor()->GetName());
			if (hit.GetActor()->GetClass()->IsChildOf(APickUp::StaticClass())) {
				//hit.GetActor()->Destroy(); 
				//TODO Pickup Actor
				CurrentItem = Cast<APickUp>(hit.GetActor());



				//UE_LOG(LogTemp, Warning, TEXT("%s"), hit.GetActor()->GetClass()->GetName() );

				//UE_LOG(LogTemp, Warning, TEXT("hitit"));
			}
			else {
				CurrentItem = NULL;
			}
			if (CurrentItem != nullptr) {
				FString ItemName = CurrentItem->GetName();
				//UE_LOG(LogTemp, Warning, TEXT("You are already carrying. %s"), *ItemName);
			}
		}
		else
		{
			CurrentItem = NULL;
			if (CurrentInView) {
				if (CurrentInView->GetClass()->IsChildOf(AStaticInteractable::StaticClass())) {
					Cast<AStaticInteractable>(CurrentInView)->bInView = false;
				}
			}
		}

	}

	if (bInspecting) {
		if (bHoldingPickUp) {
			Camera->SetFieldOfView(FMath::Lerp(Camera->FieldOfView, 90.0f, 0.1f));

			HoldingComponent->SetRelativeLocation(FVector(80.0f, 0.0f, 0.0f));
			StickHoldingComponent->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
			StickHoldingComponent->SetRelativeRotation(FQuat(0, 0, 0, 0));
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 179.9000000000002f;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = -179.9000000000002f;
			CurrentItem->InspectActor();
			if (CurrentItem) {
				CurrentItem->bInspecting = true;
			}
		}
		else {
			if (!bPhotoCamera)
				Camera->SetFieldOfView(FMath::Lerp(Camera->FieldOfView, 45.0f, 0.1f));
		}


	}
	else {
		if (CurrentItem) {
			CurrentItem->bInspecting = false;
		}
		Camera->SetFieldOfView(FMath::Lerp(Camera->FieldOfView, 90.0f, 0.1f));
		if (bHoldingPickUp) {
			HoldingComponent->SetRelativeLocation(HoldingPosition);
			StickHoldingComponent->SetRelativeTransform(StickHoldingTransform);
		}
	}


	if (CurrentItem && CurrentItem->wieldable && bFire) {
		StickHoldingComponent->RelativeRotation.Pitch += whackspeed;
		if (StickHoldingComponent->RelativeRotation.Pitch >= whacklimit) {
			bBacklash = true;
		}
		if (bBacklash) {
			StickHoldingComponent->RelativeRotation.Pitch -= whackspeed;
			if (StickHoldingComponent->RelativeRotation.Pitch <= StartWhackRotation) {
				bBacklash = false;
				bFire = false;
			}
		}
	}

	if (bFreeView) {
		GetWorld()->GetFirstPlayerController()->InputPitchScale = MouseSensY * (-1);
		GetWorld()->GetFirstPlayerController()->InputYawScale = MouseSensX;
	}
	if (!bFreeView) {
		GetWorld()->GetFirstPlayerController()->InputPitchScale = 0;
		GetWorld()->GetFirstPlayerController()->InputYawScale = 0;
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
	FVector EndTrace = ((ForwardVector*500.f) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
	//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, true);
	return GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams);



}






void AFPSwanderfulCharacter::Interact()
{

	if (bFilmRoll) {
		Cast<AFilmRoll>(hit.GetActor())->OnPickUp();
	}
	if (!bPhotoCamera) {
		InteractPressed = true;
		if (CurrentItem && !bInspecting) {
			ToggleItemPU();
		}
	}

	/*if (CastRay() != nullptr) {
	if (CastRay()->GetActor()->GetName() == "PickUp1") {
	CastRay()->GetActor()->Destroy();
	}
	}*/
	//UE_LOG(LogTemp, Warning, TEXT("Interacted"));
}



void AFPSwanderfulCharacter::OnInspect()
{

	if (bHoldingPickUp) {
		LastRotation = GetControlRotation();
		ToggleMovement();
		InspectBlur->bEnabled = true;
	}
	else {
		bInspecting = true;
	}
}


void AFPSwanderfulCharacter::OnInspectReleased()
{


	if (bInspecting && bHoldingPickUp) {
		GetController()->SetControlRotation(LastRotation);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = rotationMax;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = rotationMin;
		InspectBlur->bEnabled = false;
		ToggleMovement();

	}
	else {
		bInspecting = false;
	}

}

void AFPSwanderfulCharacter::OnFiring()
{
	if (CurrentItem) {
		if (CurrentItem->wieldable)
			bFire = true;
	}

}

void AFPSwanderfulCharacter::ToggleMovement()
{
	bCanMove = !bCanMove;
	bInspecting = !bInspecting;
	Camera->bUsePawnControlRotation = !Camera->bUsePawnControlRotation;
	bUseControllerRotationYaw = !bUseControllerRotationYaw;
}

void AFPSwanderfulCharacter::ToggleItemPU()
{
	if (CurrentItem) {

		bHoldingPickUp = !bHoldingPickUp;
		CurrentItem->PickUpObject();
		Inventory = CurrentItem;
		if (!bHoldingPickUp) {
			if (!Cast<APickUp>(Inventory)->puzzlepu) {
				Inventory = NULL;
				CurrentItem = NULL;
			}
		}
	}
}





// Called to bind functionality to input
void AFPSwanderfulCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	//Basic Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSwanderfulCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSwanderfulCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AFPSwanderfulCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSwanderfulCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPSwanderfulCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPSwanderfulCharacter::AddControllerPitchInput);


	//jumping
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSwanderfulCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this,&AFPSwanderfulCharacter::StopJump);

	//interaction
	//PlayerInputComponent->BindAction("InteractMain", IE_Pressed, this, &AFPSwanderfulCharacter::Interact);
	//PlayerInputComponent->BindAction("InteractSide", IE_Pressed, this, &AFPSwanderfulCharacter::Drop);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFPSwanderfulCharacter::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AFPSwanderfulCharacter::InteractEnd);
	PlayerInputComponent->BindAction("Inspect", IE_Pressed, this, &AFPSwanderfulCharacter::OnInspect);
	PlayerInputComponent->BindAction("Inspect", IE_Released, this, &AFPSwanderfulCharacter::OnInspectReleased);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSwanderfulCharacter::OnFiring);

}


void AFPSwanderfulCharacter::MoveForward(float value)
{
	//find out forward direction
	if (bCanMove) {
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, value);
		if (abs(value) > 0 && !bBobStrafe) {
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(MyShake, 0.08f);
			bBobStraight = true;
		}
		else {
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StopCameraShake(MyShakeUC, true);
			bBobStraight = false;
		}
	}
}

void AFPSwanderfulCharacter::MoveRight(float value)
{
	if (bCanMove) {
		//find out right direction
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, value);
		if (abs(value) > 0 && !bBobStraight) {
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(MyShake, 0.08f);
			bBobStrafe = true;
		}
		else {
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StopCameraShake(MyShakeUC, true);
			bBobStrafe = false;
		}
	}
}

void AFPSwanderfulCharacter::StartJump()
{
	if(bCanMove)
	bPressedJump = true;
}

void AFPSwanderfulCharacter::StopJump()
{
	if(bCanMove)
	bPressedJump = false;
}



