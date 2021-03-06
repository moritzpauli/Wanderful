// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WanderfulHUD.generated.h"

UCLASS()
class AWanderfulHUD : public AHUD
{
	GENERATED_BODY()

public:
	AWanderfulHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
	FVector2D OGSize;
	bool bGetPlayer;
	TArray<AActor*> MyPlayers;
	class AFPSwanderfulCharacter* MyPlayer;

};

