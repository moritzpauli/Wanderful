// Fill out your copyright notice in the Description page of Project Settings.

#include "PhotoCamera.h"
#include "Engine.h"
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "FPSwanderfulCharacter.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameSaves.h"
#include "TextWidget.h"



// Sets default values
APhotoCamera::APhotoCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Testpath = TEXT("D:/Pictures/WanderfulScreenshot1.png");
	PictureCounter = 1;
	bCameraMode = false;
	filmstock = 24;
	bFocusBlur = false;
	bFocused = false;
}

// Called when the game starts or when spawned
void APhotoCamera::BeginPlay()
{
	Super::BeginPlay();
	EnableInput(GetWorld()->GetFirstPlayerController());
	UInputComponent* PhotoInputComponent = this->InputComponent;
	if (PhotoInputComponent) {
		SetupPhotoInputComponent(PhotoInputComponent);
	}
	PathUpictures = TEXT("C:/Users/");
	TCHAR Username[256 + 1];
	DWORD Username_len = 256 + 1;
	GetUserName(Username, &Username_len);
	FString MyUsername = Username;
	//UE_LOG(LogTemp, Warning, TEXT("This user's name is %s"), *MyUsername);
	PathUpictures.Append(MyUsername);
	PathUpictures.Append(TEXT("/Pictures/Wanderful/AWanderfulPhotograph"));
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPSwanderfulCharacter::StaticClass(), MyPlayers);
	MyPlayer = Cast<AFPSwanderfulCharacter>(MyPlayers[0]);
	PlayerCam = MyPlayer->Camera;
	OgPostProcessing = PlayerCam->PostProcessSettings;
	SaveGameInstance = Cast<UGameSaves>(UGameplayStatics::CreateSaveGameObject(UGameSaves::StaticClass()));
	if (UGameplayStatics::DoesSaveGameExist(TEXT("MySlot"), 0)) {
		SaveGameInstance = Cast<UGameSaves>(UGameplayStatics::LoadGameFromSlot("MySlot", 0));
		PictureCounter = SaveGameInstance->ImageIndex;
	}

}

// Called every frame
void APhotoCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bFocusBlur) {
		//PhotoPostProcessing->BlendWeight = 1.0f;
	}

}


void APhotoCamera::TakePhoto() {
	if (bCameraMode && filmstock > 0) {

		if (bFocused) {
			UE_LOG(LogTemp, Warning, TEXT("klick klack"));
			PathUpictures.Append(FString::FromInt(PictureCounter));
			PathUpictures.Append(TEXT(".png"));
			FScreenshotRequest::RequestScreenshot(PathUpictures, false, false);
			char* CharPath = TCHAR_TO_ANSI(*PathUpictures);
			for (int i = 0; i < PathUpictures.Len(); i++) {
				if (CharPath[i] == '1' || CharPath[i] == '2' || CharPath[i] == '3' || CharPath[i] == '4' || CharPath[i] == '5' ||
					CharPath[i] == '6' || CharPath[i] == '7' || CharPath[i] == '8' || CharPath[i] == '9') {
					PathUpictures.RemoveAt(i, PathUpictures.Len() - i);
					UE_LOG(LogTemp, Warning, TEXT("%d"), PictureCounter);
				}
			}
			PictureCounter++;
			filmstock--;
			SaveGameInstance->ImageIndex = PictureCounter;
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot"), 0);
			bFocused = false;
		}


	}

}

void APhotoCamera::EnterCameraMode()
{
	bCameraMode = !bCameraMode;
	if (bCameraMode && MyPlayer->CurrentItem == NULL && MyPlayer->bCanMove == true) {
		UE_LOG(LogTemp, Warning, TEXT("PhotoModeStart"));
		PlayerCam->PostProcessSettings = PhotoPostProcessing;
		MyPlayer->bPhotoCamera = true;
		CameraOverlay->AddToViewport();
		//CameraOverlay->SetDisplayText(TEXT("fuck"));
	}
	if (!bCameraMode) {
		PlayerCam->PostProcessSettings = OgPostProcessing;
		MyPlayer->bPhotoCamera = false;
		CameraOverlay->RemoveFromViewport();
	}
}

void APhotoCamera::ExitCameraMode()
{
}

void APhotoCamera::DeleteSave()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("MySlot"), 0)) {
		SaveGameInstance->ImageIndex = 0;
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot"), 0);
		SaveGameInstance = Cast<UGameSaves>(UGameplayStatics::LoadGameFromSlot("MySlot", 0));
		PictureCounter = SaveGameInstance->ImageIndex;
	}
}

void APhotoCamera::SetupPhotoInputComponent(UInputComponent* PhotoInputComponent)
{

	PhotoInputComponent->BindAction("Photo", IE_Pressed, this, &APhotoCamera::TakePhoto);
	PhotoInputComponent->BindAction("CameraMode", IE_Pressed, this, &APhotoCamera::EnterCameraMode);
	PhotoInputComponent->BindAction("CameraMode", IE_Released, this, &APhotoCamera::ExitCameraMode);
	PhotoInputComponent->BindAction("DeleteSave", IE_Pressed, this, &APhotoCamera::DeleteSave);

}

