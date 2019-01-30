// Fill out your copyright notice in the Description page of Project Settings.

#include "FishingSpot.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "FPSwanderfulCharacter.h"
#include "Engine.h"
#include "Fish.h"
#include "CableComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"

AFishingSpot::AFishingSpot() {
	Fishtank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyFishtank"));
	Float = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyFloat"));
	FishtankPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyFishtankPlane"));
	WireConnector = CreateDefaultSubobject<USceneComponent>(TEXT("WireConnector"));
	RespawnTank = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RespawnTank"));
	HookTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("HookTrigger"));
	//FloatToFish = CreateDefaultSubobject<UCableComponent>(TEXT("FloatToFish"));
	//PhysicsFloatToFish = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsFloatToFish"));

	//PhysicsFloatToFish->SetupAttachment(Float);
	//FloatToFish->SetupAttachment(PhysicsFloatToFish);
	WireConnector->SetupAttachment(Float);
	HookTrigger->SetupAttachment(Float);
	HookTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFishingSpot::OnHookOverlapBegin);

	bHookCast = false;
	bHooked = false;
	bOnInteractEnd = false;
	bOnInteractStart = false;
	bSpawnFish = false;
	bFloatpull = true;
	bStruggle = true;
	bCaught = false;
	lerpin = true;
	bGetHookPos = false;

}


void AFishingSpot::BeginPlay()
{
	Super::BeginPlay();
	FishtankPlane->SetWorldTransform(Fishtank->GetComponentTransform());
	FishtankPlane->SetWorldScale3D(FVector(FishtankPlane->GetComponentScale().X, FishtankPlane->GetComponentScale().Y, 0.01f));
	FishtankPlane->AddLocalOffset(FVector(0.0f, 0.0f, (Fishtank->CalcBounds(PlaneTransform).BoxExtent.Z))*Fishtank->RelativeScale3D.Z);
	FishtankPlane->SetVisibility(false);
	RespawnTank->SetWorldTransform(Fishtank->GetComponentTransform());
	RespawnTank->SetWorldScale3D(FVector(RespawnTank->GetComponentScale().X*1.7f, RespawnTank->GetComponentScale().Y*1.7f, RespawnTank->GetComponentScale().Z));
	myMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SpawnedFish.SetNum(FishAmount);
	RespawnTank->SetCollisionProfileName("ObjectOverlap");
	RespawnTank->SetVisibility(false);
	HookTrigger->SetCollisionProfileName("Trigger");
	//FloatToFish->SetHiddenInGame(true);
	FloatStartPosition = Float->GetComponentLocation();

}




void AFishingSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetBInteract()) {
		if (HookedFish) {
			CurrentFishDestroyable = HookedFish->bDestroyable;
		}
		if (!bSpawnFish) {
			for (int i = 0; i < FishAmount; i++) {
				SpawnFish(i);
			}
			bSpawnFish = true;
		}
		if (!bOnInteractStart) {
			myMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			FishtankPlane->SetVisibility(true);
			Float->SetVisibility(true);
			bOnInteractEnd = false;
			bOnInteractStart = true;
		}
		MyPlayer->FishingRodOn();
		if (bTankInView) {
			Float->SetWorldLocation(FloatPosition);
		}
		if (bHookCast) {
			if (!bHooked) {
				FVector NewLocation = Float->GetComponentLocation();
				float DeltaHeight = (FMath::Sin((RunningTime - DeltaTime)*1.0f) - FMath::Sin(RunningTime*1.0f));
				NewLocation.Z += DeltaHeight * 6.0f;
				RunningTime += DeltaTime;
				FloatPosition = NewLocation;
			}
		}
		if (bHooked) {
			//bit bob
			OgFloatPos = Float->GetComponentLocation();
			if (bFloatpull) {
				FVector NewLocation = Float->GetComponentLocation();
				float DeltaHeight = (FMath::Sin((PullTime - DeltaTime)*5.0f) - FMath::Sin(PullTime*5.0f));
				if (!countstop && DeltaHeight >= 0) {
					bobcount++;
					countstop = true;
				}
				if (countstop && DeltaHeight < 0) {
					countstop = false;
				}
				NewLocation.Z += DeltaHeight * 10.0f;
				PullTime += DeltaTime;
				FloatPosition = NewLocation;
				if (bobcount > 0) {
					if (FloatPosition.Z < OgFloatPos.Z) {
						PullTime = 0.0f;
						bFloatpull = false;

					}
				}
			}
			if (!bFloatpull) {
				if (strugglecounter <= HookedFish->Toughness) {
					struggletimer -= DeltaTime;
					if (bStruggle) {
						Balance -= HookedFish->Strength*DeltaTime;
					}
					Balance += ReelSpeed;
					float DeltaHeight = (FMath::Sin((RunningTime - DeltaTime)*1.0f) - FMath::Sin(RunningTime*1.0f))*6.0f;
					RunningTime += DeltaTime;
					float FloatPosZ = Float->GetComponentLocation().Z + DeltaHeight + (Balance*FloatWeight);
					Float->SetWorldLocation(FVector(Float->GetComponentLocation().X, Float->GetComponentLocation().Y, FloatPosZ));

					if (struggletimer <= 0) {
						bStruggle = !bStruggle;
						strugglecounter++;
						FString toprint = TEXT("the fish got weaker");
						toprint.AppendInt(strugglecounter);
						UE_LOG(LogTemp, Warning, TEXT("%s"), *toprint);
						if (FMath::RandRange(0.0f, 1.0f) <= 0.3f) {
							HookedFish->Strength += HookedFish->Strength / 16;
						}
						struggletimer = FMath::RandRange(StruggleTimeMin, StruggleTimeMax);
					}
				}


				if (strugglecounter > HookedFish->Toughness) {
					if (lerpin) {
						if (!bGetHookPos) {
							OgHookPos = Float->GetComponentLocation();
							HookedFish->AttachToComponent(Float, FAttachmentTransformRules::KeepWorldTransform);
							bGetHookPos = true;
						}
						ReelSpeed = ReelSpeed - ReelSpeed / 3;
						progress = progress + ReelSpeed * -1 * DeltaTime*40.0f;
						Float->SetWorldLocation(FMath::Lerp(OgHookPos, FVector(MyPlayer->RodTip->GetComponentLocation().X, MyPlayer->RodTip->GetComponentLocation().Y,
							Float->GetComponentLocation().Z), progress));
						//UE_LOG(LogTemp, Warning, TEXT("On the Hook"));
						HookedFish->bDestroyable = false;
						if (progress >= 0.8f) {
							if (!bCaught) {
								FishCatch();
							}
						}
					}

				}
				if (Balance < -ScaleMax || Balance>ScaleMax) {
					FishFree();
				}
			}
		}

	}
	if (!GetBInteract()) {
		
		if (!bOnInteractEnd) {
			MyPlayer->FishingRodOff();
			myMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			FishtankPlane->SetVisibility(false);
			Float->SetVisibility(false);
			if (HookedFish) {
				HookedFish->bMove = true;
				HookedFish->Destroy();
				HookedFish = nullptr;

			}
			bHookCast = false;
			bHooked = false;
			bStruggle = true;
			bobcount = 0;
			bFloatpull = true;
			countstop = false;
			bOnInteractStart = false;
			
			/*PhysicsFloatToFish->ConstraintActor2 = NULL;
			PhysicsFloatToFish->ComponentName2.ComponentName = FName("");*/
			bCaught = false;
			lerpin = true;
			progress = 0.0f;
			bGetHookPos = false;
			bOnInteractEnd = true;
		}

	}

}


//casting out the hook
void AFishingSpot::ToggleCastHook()
{
	if (!bHooked) {
		bHookCast = !bHookCast;
		if (bHookCast)
			FishtankPlane->SetVisibility(false);
		if (!bHookCast) {
			FishtankPlane->SetVisibility(true);
			bFloatpull = true;
			Float->SetWorldLocation(FVector(Float->GetComponentLocation().X,Float->GetComponentLocation().Y,FloatStartPosition.Z));
			bHooked = false;
			if (HookedFish) {
				HookedFish->bMove = true;
				HookedFish = nullptr;
			}
		}
	}


}

//when the fish frees itself
void AFishingSpot::FishFree()
{
	UE_LOG(LogTemp, Warning, TEXT("The Fish freed itself"));

	HookedFish->bDestroyable = true;
	HookedFish->bMove = true;
	bStruggle = true;
	bFloatpull = true;
	bHooked = false;
	ToggleCastHook();
	strugglecounter = 0;
	ReelSpeed = 0.0f;
	HookedFish = nullptr;
}

void AFishingSpot::FishCatch()
{
	lerpin = false;
	HookedFish->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	HookedFish->RootMesh->SetSimulatePhysics(true);
	HookedFish->AddActorWorldOffset(FVector(0,0,10));
	Float->SetVisibility(false);
	//FloatToFish->SetHiddenInGame(false);
	//FloatToFish->SetAttachEndTo(HookedFish, TEXT("Mouthspot"), TEXT(""));
	//PhysicsFloatToFish->ConstraintActor2 = HookedFish;
	//PhysicsFloatToFish->ComponentName2.ComponentName = FName("Mouthspot");

	//test
	//MyPlayer->RodTip->ConstraintActor2 = TestSphere;
	

	
	MyPlayer->RodTip->ConstraintActor2 = HookedFish;
	MyPlayer->RodTip->ComponentName2.ComponentName = FName("Mouthspot");
	

	
	bStruggle = true;
	bFloatpull = true;
	strugglecounter = 0;
	ReelSpeed = 0.0f;
	bCaught = true;
	bHooked = false;

}

//Getting Input from Playercharacter
void AFishingSpot::ReceiveReelInput(int polarity)
{
	if (bHooked && !bFloatpull) {
		ReelSpeed = ReelSpeed + (MouseWheelSens*polarity);
	}
}


void AFishingSpot::SpawnFish(int Index)
{
	int variant = FMath::RandRange(0, FishVariants.Num() - 1);
	if (FishVariants[variant] != NULL) {
		FActorSpawnParameters FishSpawnParam;
		Fishtank->GetLocalBounds(TankBoundsMin, TankBoundsMax);
		FishSpawnParam.Template = FishVariants[variant];
		SpawnedFish[Index] = GetWorld()->SpawnActor<AFish>(FishVariants[variant]->GetClass(), FVector(0, 0, 0), FRotator::ZeroRotator, FishSpawnParam);
		SpawnedFish[Index]->Fishdex = Index;
		SpawnedFish[Index]->GetRootComponent()->AttachTo(Fishtank);
		SpawnedFish[Index]->SetActorRelativeLocation(FVector(0, 0, 0));
		//SpawnedFish[i]->SetActorRelativeRotation(FRotator::ZeroRotator);
		SpawnedFish[Index]->SetActorScale3D(FishVariants[variant]->GetActorScale());
		FishBounds = SpawnedFish[Index]->GetRootComponent()->CalcBounds(FishTransform).BoxExtent*SpawnedFish[Index]->GetRootComponent()->RelativeScale3D;
		TankBounds = Fishtank->CalcBounds(TankTransform).BoxExtent;
		float OffsetY = FMath::RandRange(-(Fishtank->CalcBounds(PlaneTransform).BoxExtent.Y) + FishBounds.Y + 0.5f, (Fishtank->CalcBounds(PlaneTransform).BoxExtent.Y) - FishBounds.Y - 0.5f);
		float OffsetX = FMath::RandRange(-(Fishtank->CalcBounds(PlaneTransform).BoxExtent.X) + FishBounds.X + 0.5f, (Fishtank->CalcBounds(PlaneTransform).BoxExtent.X) - FishBounds.X - 0.5f);
		SpawnedFish[Index]->AddActorLocalOffset(FVector(OffsetX, OffsetY, 0));
		SpawnedFish[Index]->SetActorHiddenInGame(false);
	}
}



void AFishingSpot::OnHookOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp && bHookCast && !bHooked) {
		if (OtherActor->GetClass()->IsChildOf(AFish::StaticClass())) {
			if (!(Cast<AFish>(OtherActor)->bIdle)) {
				UE_LOG(LogTemp, Warning, TEXT("Swam by Hook"));
				if (FMath::RandRange(0.0f, 1.0f) <= (Cast<AFish>(OtherActor)->HookingProb)) {
					HookedFish = Cast<AFish>(OtherActor);
					bHooked = true;
					Cast<AFish>(OtherActor)->bMove = false;
					struggletimer = FMath::RandRange(StruggleTimeMin, StruggleTimeMax);
					Balance = 0.0f;
				}
			}

		}
	}

}




