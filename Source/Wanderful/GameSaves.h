// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSaves.generated.h"

/**
 * 
 */
UCLASS()
class WANDERFUL_API UGameSaves : public USaveGame
{
	GENERATED_BODY()
	
public:
	UGameSaves();

	UPROPERTY()
	int ImageIndex;
	
	
};
