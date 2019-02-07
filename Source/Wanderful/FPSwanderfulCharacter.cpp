// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSwanderfulCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine.h"
#include "Pickup.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FilmRoll.h"
#include "PuzzleItemSpot.h"
#include "FishingSpot.h"
#include "CableComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"
#include "Fish.h"
#include "PhotoCamera.h"
#include "FloodGate.h"









// Sets default values
AFPSwanderfulCharacter::AFPSwanderfulCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	HoldingComponent = CreateDefaultSubobject<USceneComponent>("HoldingComponent");
	HoldCamera = CreateDefaultSubobject<UStaticMeshComponent>("HoldCamera");
	StickHoldingComponent = CreateDefaultSubobject<USceneComponent>("StickHoldingComponent");
	FishingRod = CreateDefaultSubobject<UStaticMeshComponent>("FishingRod");
	RodTip = CreateDefaultSubobject<UPhysicsConstraintComponent>("RodTip");
	LineStartComponent = CreateDefaultSubobject<UStaticMeshComponent>("LineStartComponent");
	LineStartPosition = CreateDefaultSubobject<USceneComponent>("LineStartPosition");
	FishInspectComponent = CreateDefaultSubobject<USceneComponent>("FishInspectComponent");

	FishInspectComponent->SetupAttachment(Camera);
	FishingRod->SetupAttachment(Camera);
	FishingWire = CreateDefaultSubobject<UCableComponent>("MyFishingWire");
	FishingWire->SetupAttachment(FishingRod);
	LineStartComponent->SetupAttachment(FishingRod);
	RodTip->SetupAttachment(FishingRod);
	HoldingComponent->SetRelativeLocation(HoldingPosition);
	HoldingComponent->SetupAttachment(Camera);
	HoldCamera->SetupAttachment(Camera);
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
	RayCastLength = 800.0f;
	FishingRod->SetActive(false);
	bCanMove = true;
	CameraHoldingPosition = FVector(40, 15, -35);
	bFishInspect = false;
	bCountedCatchI = false;
	MyCatches.SetNum(100);
	bFishExitInspect = false;
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
	FishingRod->SetVisibility(false);
	FishingWire->SetVisibility(false);
	FishingRod->Deactivate();
	FishingWire->Deactivate();
	HoldCamera->SetHiddenInGame(true);
	RodTip->ConstraintActor1 = this;
	RodTip->ComponentName1.ComponentName = FName("LineStartComponent");
	HoldCamera->SetRelativeLocation(CameraHoldingPosition);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APhotoCamera::StaticClass(), LevelPhotoCameras);
	LevelPhotoCamera = Cast<APhotoCamera>(LevelPhotoCameras[0]);
	LineStartPosition->SetWorldLocation(LineStartComponent->GetComponentLocation());
	LineStartPosition->SetupAttachment(FishingRod);
	OgCableLength = FishingWire->CableLength;
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
		if (CurrentInView) {
			if (CurrentInView->GetClass()->IsChildOf(AFishingSpot::StaticClass())) {
				if (Cast<AFishingSpot>(CurrentInView)->bInteracting &&Cast<AFishingSpot>(CurrentInView)->bCaught) {
					FishingWire->SetAttachEndTo(Cast<AFishingSpot>(CurrentInView)->HookedFish, TEXT("MouthSpot"), TEXT(""));
				}


			}
		}
		if (CastRay(hit)) {



			if (hit.GetActor()->GetClass()->IsChildOf(AFilmRoll::StaticClass()) && !bPhotoCamera) {
				bFilmRoll = true;
			}
			else {
				bFilmRoll = false;
			}

			FString compName = hit.GetComponent()->GetName();
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *compName);
			//Fish Tank in View
			if (hit.GetComponent()->GetName() == TEXT("MyFishtankPlane")) {


				CurrentInView = hit.GetActor();
				Cast<AFishingSpot>(CurrentInView)->bTankInView = true;

				if (!Cast<AFishingSpot>(CurrentInView)->bHookCast && !Cast<AFishingSpot>(CurrentInView)->bCaught) {
					Cast<AFishingSpot>(CurrentInView)->FloatPosition = hit.ImpactPoint;
				}


				if (!Cast<AFishingSpot>(CurrentInView)->bCaught) {
					FishingWire->SetAttachEndTo(CurrentInView, TEXT("WireConnector"), TEXT(""));
				}
			}
			else {
				if (CurrentInView) {
					if (hit.GetComponent()->GetName() == TEXT("MyFishtankPlane")) {
						Cast<AFishingSpot>(CurrentInView)->bTankInView = false;

					}
				}
			}



			//Static Interactable in View
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
				if (CurrentInView->GetClass()->IsChildOf(AFishingSpot::StaticClass())) {
					Cast<AFishingSpot>(CurrentInView)->bTankInView = false;

				}
			}

		}

	}



	if (bInspecting) {
		if (bFishInspect) {
			Camera->SetFieldOfView(FMath::Lerp(Camera->FieldOfView, 90.0f, 6.0f*DeltaTime));
			MyCatches[CatchIndex]->ShowMesh->SetWorldLocation(FishInspectComponent->GetComponentLocation());
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 179.9000000000002f;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = -179.9000000000002f;
			ControlRotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
			MyCatches[CatchIndex]->ShowMesh->SetWorldRotation(FQuat(ControlRotation));
			MyCatches[CatchIndex]->ShowMesh->SetRenderCustomDepth(true);
		}
		if (bHoldingPickUp) {
			Camera->SetFieldOfView(FMath::Lerp(Camera->FieldOfView, 90.0f, 6.0f*DeltaTime));
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
		if (bCameraInHand) {
			Camera->SetFieldOfView(FMath::Lerp(Camera->FieldOfView, 90.0f, 6.0f*DeltaTime));
			HoldCamera->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 179.9000000000002f;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = -179.9000000000002f;
			ControlRotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
			HoldCamera->SetWorldRotation(FQuat(ControlRotation));
			HoldCamera->SetRenderCustomDepth(true);
		}
		else {
			if (!bPhotoCamera && !bHoldingPickUp && !bFishInspect) {
				Camera->SetFieldOfView(FMath::Lerp(Camera->FieldOfView, 45.0f, 0.1f));
			}
		}


	}
	else {
		if (CurrentItem) {
			CurrentItem->bInspecting = false;
		}
		Camera->SetFieldOfView(FMath::Lerp(Camera->FieldOfView, 90.0f, 6.0f*DeltaTime));
		if (bHoldingPickUp) {
			HoldingComponent->SetRelativeLocation(HoldingPosition);
			StickHoldingComponent->SetRelativeTransform(StickHoldingTransform);
		}
		if (bCameraInHand) {
			HoldCamera->SetRelativeLocation(CameraHoldingPosition);
			HoldCamera->SetRelativeRotation(FRotator(0, 0, 0));
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
	FVector EndTrace = ((ForwardVector*RayCastLength) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
	//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, true);
	return GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams);



}






void AFPSwanderfulCharacter::Interact()
{
	if (bFishInspect) {
		MyCatches[CatchIndex]->DefaultRoot->SetWorldLocation(FVector(-9999, -9999, -9999));
		MyCatches[CatchIndex]->ShowMesh->SetWorldLocation(FVector(-9999, -9999, -9999));
		MyCatches[CatchIndex]->RootMesh->SetWorldLocation(FVector(-9999, -9999, -9999));
		
		CatchIndex++;
		bFishExitInspect = true;
		OnInspectReleased();
		bFishExitInspect = false;
		bFishInspect = false;
		Cast<AFishingSpot>(CurrentInView)->bInRange = true;

	}
	if (bFilmRoll) {
		Cast<AFilmRoll>(hit.GetActor())->OnPickUp();
	}
	if (!bPhotoCamera && !bFishInspect) {
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
	if (!bFishInspect) {
		if (bHoldingPickUp || bCameraInHand) {

			LastRotation = GetControlRotation();
			ToggleMovement();
			InspectBlur->bEnabled = true;
		}
		else {
			bInspecting = true;
			UE_LOG(LogTemp, Warning, TEXT("oninspectelse"));
		}
	}

	if (bFishInspect) {
		if (bFishEnterInspect && !bFishExitInspect) {

			LastRotation = GetControlRotation();
			ToggleMovement();
			InspectBlur->bEnabled = true;
		}
		else {
			if (!bFishExitInspect) {
				bInspecting = true;
				UE_LOG(LogTemp, Warning, TEXT("oninspectelse"));
			}
		}
	}



}


void AFPSwanderfulCharacter::OnInspectReleased()
{
	if (!bFishInspect) {
		if ((bInspecting && bHoldingPickUp) || (bInspecting && bCameraInHand)) {
			UE_LOG(LogTemp, Warning, TEXT("oninspectreleased"));
			GetController()->SetControlRotation(LastRotation);
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = rotationMax;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = rotationMin;
			InspectBlur->bEnabled = false;
			ToggleMovement();
		}
		else {
			bInspecting = false;
			UE_LOG(LogTemp, Warning, TEXT("oninspectreleasedelse"));
		}
	}
	if (bFishInspect) {
		if ((bInspecting && bFishExitInspect)) {
			UE_LOG(LogTemp, Warning, TEXT("oninspectreleased"));
			GetController()->SetControlRotation(LastRotation);
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = rotationMax;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = rotationMin;
			InspectBlur->bEnabled = false;
			ToggleMovement();
		}
		else {
			if (bFishExitInspect) {
				bInspecting = false;
				UE_LOG(LogTemp, Warning, TEXT("oninspectreleasedelse"));
			}
		}
	}



}

void AFPSwanderfulCharacter::OnFiring()
{
	if (CurrentItem) {
		if (CurrentItem->wieldable)
			bFire = true;
	}
	if (CurrentInView) {
		if (CurrentInView->GetClass()->IsChildOf(AFishingSpot::StaticClass())) {
			if (Cast<AFishingSpot>(CurrentInView)->bInteracting) {
				if (!Cast<AFishingSpot>(CurrentInView)->bCaught) {
					Cast<AFishingSpot>(CurrentInView)->ToggleCastHook();
				}
			}
		}
	}
	if (bPhotoCamera) {
		//fire shot
		if (LevelPhotoCamera) {
			LevelPhotoCamera->TakePhoto();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("No Phot Camera in this Level!"));
		}
	}

}

void AFPSwanderfulCharacter::OnWheelUp()
{
	if (CurrentInView) {
		if (CurrentInView->GetClass()->IsChildOf(AFishingSpot::StaticClass())) {
			if (Cast<AFishingSpot>(CurrentInView)->bInteracting) {
				Cast<AFishingSpot>(CurrentInView)->ReceiveReelInput(1);
			}
		}
		if (CurrentInView->GetClass()->IsChildOf(AFloodGate::StaticClass())) {
			if (Cast<AFloodGate>(CurrentInView)->bInteracting) {
				Cast<AFloodGate>(CurrentInView)->TurnCrank(-1);
			}
		}
	}
}

void AFPSwanderfulCharacter::OnWheelDown()
{
	if (CurrentInView) {
		if (CurrentInView->GetClass()->IsChildOf(AFishingSpot::StaticClass())) {
			if (Cast<AFishingSpot>(CurrentInView)->bInteracting) {
				Cast<AFishingSpot>(CurrentInView)->ReceiveReelInput(-1);
				if (Cast<AFishingSpot>(CurrentInView)->bCaught) {
					LineStartComponent->AddWorldOffset(FVector(0, 0, 2.5f));
					FishingWire->CableLength -= 2.5f;
					if (Cast<AFishingSpot>(CurrentInView)->HookedFish->RootMesh->GetComponentLocation().Z >= LineStartPosition->GetComponentLocation().Z - 30) {
						FishingComplete();

					}
				}
			}
		}
		if (CurrentInView->GetClass()->IsChildOf(AFloodGate::StaticClass())) {
			if (Cast<AFloodGate>(CurrentInView)->bInteracting) {
				Cast<AFloodGate>(CurrentInView)->TurnCrank(1);
			}
		}
	}
	if (bFishingTest) {
		LineStartComponent->AddWorldOffset(FVector(0, 0, 1.0f));
	}
}

void AFPSwanderfulCharacter::OnFishingTest()
{
	/*UE_LOG(LogTemp, Warning, TEXT("enter fishtest"));
	bFishingTest = !bFishingTest;
	if (bFishingTest) {
		FishingRodOn();
		if (FishingTestActor && !ConstraintInitialized) {
			FishingWire->SetAttachEndTo(FishingTestActor, TEXT(""), TEXT(""));
			RodTip->ConstraintActor2 = FishingTestActor;
			FString RootCompName = FishingTestActor->GetRootComponent()->GetName();
			RodTip->ComponentName2.ComponentName = FName(*RootCompName);
			RodTip->InitComponentConstraint();
			ConstraintInitialized = true;

		}

	}
	if (!bFishingTest) {
		ConstraintInitialized = false;
		FishingRodOff();
	}*/
}

void AFPSwanderfulCharacter::OnTakeCamera()
{
	AStaticInteractable* MyInteractable;
	bool InInteract = false;
	if (CurrentInView) {
		if (CurrentInView->GetClass()->IsChildOf(AStaticInteractable::StaticClass())) {
			MyInteractable = Cast<AStaticInteractable>(CurrentInView);
			InInteract = MyInteractable->bInteracting;
		}
	}
	if (!bPhotoCamera && !bInspecting && !InInteract) {
		bCameraInHand = !bCameraInHand;
		if (bCameraInHand) {
			if (bHoldingPickUp) {
				if (CurrentItem) {
					CurrentItem->PickUpObject();
					bHoldingPickUp = false;
				}
			}
			HoldCamera->SetHiddenInGame(false);
		}
		if (!bCameraInHand) {
			HoldCamera->SetHiddenInGame(true);
		}
	}
}

void AFPSwanderfulCharacter::FishingComplete()
{
	if (CurrentInView && CurrentInView->GetClass()->IsChildOf(AFishingSpot::StaticClass()) && !bFishInspect) {
		if (Cast<AFishingSpot>(CurrentInView)->HookedFish) {
			Cast<AFishingSpot>(CurrentInView)->SpawnFish(Cast<AFishingSpot>(CurrentInView)->HookedFish->Fishdex);
			UE_LOG(LogTemp, Warning, TEXT("Ladies and gentlemen we got em"));
			MyCatches[CatchIndex] = Cast<AFishingSpot>(CurrentInView)->HookedFish;
			RodTip->BreakConstraint();
			MyCatches[CatchIndex]->RootMesh->SetSimulatePhysics(false);
			MyCatches[CatchIndex]->ShowMesh->SetSimulatePhysics(false);
			Cast<AFishingSpot>(CurrentInView)->HookedFish = NULL;
			Cast<AFishingSpot>(CurrentInView)->OnInteractEnd();
			FishingRodOff();
			FishingWire->CableLength = OgCableLength;
			bCountedCatchI = true;
			bFishInspect = true;
			bFishEnterInspect = true;
			OnInspect();
			bFishEnterInspect = false;
		}
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
	if (CurrentItem && !bCameraInHand) {

		bHoldingPickUp = !bHoldingPickUp;
		if (CurrentItem->stackable) {
			CurrentItem->PickUpObjectLight();
		}
		else {
			CurrentItem->PickUpObject();
		}
		Inventory = CurrentItem;
		if (!bHoldingPickUp) {
			if (CurrentItem->stackable) {
				FHitResult stackhit;
				if (CastRay(stackhit)) {
					if (stackhit.GetActor()->GetClass()->IsChildOf(APickUp::StaticClass())) {
						if ((Cast<APickUp>(stackhit.GetActor()))->stackable) {
							CurrentItem->mesh->SetWorldLocation(stackhit.Location);
							CurrentItem->mesh->SetWorldRotation(stackhit.GetActor()->GetActorRotation());
							CurrentItem->mesh->AddWorldOffset(FVector(0, 0, 55));
						}
						else
						{
							CurrentItem->ThrowObjectLight();
						}
					}
					else {
						CurrentItem->ThrowObjectLight();
					}
				}
			}
			if (!Cast<APickUp>(Inventory)->puzzlepu) {
				Inventory = NULL;
				CurrentItem = NULL;
			}
		}
	}
}

void AFPSwanderfulCharacter::FishingRodOn()
{

	FishingRod->Activate();
	FishingWire->Activate();
	FishingRod->SetVisibility(true);
	FishingWire->SetVisibility(true);

}

void AFPSwanderfulCharacter::FishingRodOff()
{
	FishingRod->SetVisibility(false);
	FishingWire->SetVisibility(false);
	FishingRod->Deactivate();
	FishingWire->Deactivate();
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
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSwanderfulCharacter::StopJump);

	//interaction
	//PlayerInputComponent->BindAction("InteractMain", IE_Pressed, this, &AFPSwanderfulCharacter::Interact);
	//PlayerInputComponent->BindAction("InteractSide", IE_Pressed, this, &AFPSwanderfulCharacter::Drop);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFPSwanderfulCharacter::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AFPSwanderfulCharacter::InteractEnd);
	PlayerInputComponent->BindAction("Inspect", IE_Pressed, this, &AFPSwanderfulCharacter::OnInspect);
	PlayerInputComponent->BindAction("Inspect", IE_Released, this, &AFPSwanderfulCharacter::OnInspectReleased);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSwanderfulCharacter::OnFiring);
	//mousewheel
	PlayerInputComponent->BindAction("WheelUp", IE_Pressed, this, &AFPSwanderfulCharacter::OnWheelUp);
	PlayerInputComponent->BindAction("WheelDown", IE_Pressed, this, &AFPSwanderfulCharacter::OnWheelDown);


	//enablefishingtest
	PlayerInputComponent->BindAction("FishingTest", IE_Pressed, this, &AFPSwanderfulCharacter::OnFishingTest);

	//takecamera
	PlayerInputComponent->BindAction("PickCamera", IE_Pressed, this, &AFPSwanderfulCharacter::OnTakeCamera);

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
	if (bCanMove)
		bPressedJump = true;
}

void AFPSwanderfulCharacter::StopJump()
{
	if (bCanMove)
		bPressedJump = false;
}



