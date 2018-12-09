// Fill out your copyright notice in the Description page of Project Settings.

#include "PhotoCamera.h"
#include "Engine.h"
#define WIN32_LEAN_AND_MEAN
#include "windows.h"


// Sets default values
APhotoCamera::APhotoCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Testpath = TEXT("D:/Pictures/WanderfulScreenshot1.png");
	PictureCounter = 1;
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
	TCHAR Username[ 256 + 1];
	DWORD Username_len = 256 + 1;
	GetUserName(Username, &Username_len);
	FString MyUsername = Username;
	//UE_LOG(LogTemp, Warning, TEXT("This user's name is %s"), *MyUsername);
	PathUpictures.Append(MyUsername);
	PathUpictures.Append(TEXT("/Pictures/Wanderful/AWanderfulPhotograph"));

	
}

// Called every frame
void APhotoCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APhotoCamera::TakePhoto() {
	UE_LOG(LogTemp,Warning,TEXT("klick klack"));
	PathUpictures.Append(FString::FromInt(PictureCounter));
	PathUpictures.Append(TEXT(".png"));
	FScreenshotRequest::RequestScreenshot(PathUpictures,false,false);
	char* CharPath = TCHAR_TO_ANSI(*PathUpictures);
	for (int i = 0; i < PathUpictures.Len(); i++) {
		if (CharPath[i] == '1' || CharPath[i] == '2' || CharPath[i] == '3' || CharPath[i] == '4' || CharPath[i] == '5' ||
			CharPath[i] == '6' || CharPath[i] == '7' || CharPath[i] == '8' || CharPath[i] == '9' ) {
			PathUpictures.RemoveAt(i,PathUpictures.Len()-i);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *PathUpictures);
		}
	}
	PictureCounter++;
	
}

void APhotoCamera::SetupPhotoInputComponent(UInputComponent* PhotoInputComponent)
{

	PhotoInputComponent->BindAction("Photo",IE_Pressed,this,&APhotoCamera::TakePhoto);
}

