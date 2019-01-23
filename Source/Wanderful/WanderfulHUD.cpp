// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "WanderfulHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "FPSwanderfulCharacter.h"
#include "Engine.h"

AWanderfulHUD::AWanderfulHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;

}


void AWanderfulHUD::DrawHUD()
{

	Super::DrawHUD();
	if (!bGetPlayer) {
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPSwanderfulCharacter::StaticClass(), MyPlayers);
		MyPlayer = Cast<AFPSwanderfulCharacter>(MyPlayers[0]);
		bGetPlayer = true;
	}
	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	OGSize = FVector2D(CrosshairTex->GetSizeX(), CrosshairTex->GetSizeY());

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition((Center.X) - (OGSize.X / 2),
		(Center.Y) - (OGSize.Y / 2));


	// draw the crosshair
	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	if (!(MyPlayer->bPhotoCamera)) {
		Canvas->DrawItem(TileItem);
	}

}
