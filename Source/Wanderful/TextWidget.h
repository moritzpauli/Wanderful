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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
	FLinearColor ImageColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
	FLinearColor ImageColorX;

	

	UFUNCTION(BlueprintPure, Category = "Widgets|Text")
	FText GetDisplayText() const;

	UFUNCTION(BlueprintPure, Category = "Widgets|Image")
	FLinearColor GetImageColor() const;

	UFUNCTION(BlueprintCallable, Category = "Widgets|Text")
	void SetDisplayText(const FText& NewDisplayText);

	UFUNCTION(BlueprintCallable, Category = "Widgets|Image")
	void SetImageColor(const FLinearColor& newColor);
	
};
