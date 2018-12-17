// Fill out your copyright notice in the Description page of Project Settings.

#include "TextWidget.h"



FText UTextWidget::GetDisplayText() const
{
	return DisplayText;
}

FLinearColor UTextWidget::GetImageColor() const
{
	return ImageColor;
}

void UTextWidget::SetDisplayText(const FText & NewDisplayText)
{
	DisplayText = NewDisplayText;
}

void UTextWidget::SetImageColor(const FLinearColor & newColor)
{
	ImageColor = newColor;
}
