// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WANDERFUL_API UTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	FText DisplayText;

	UFUNCTION(BlueprintPure, Category = "Widgets|Text")
	FText GetDisplayText() const;

	UFUNCTION(BlueprintCallable, Category = "Widgets|Text")
	void SetDisplayText(const FText& NewDisplayText);
	
};
