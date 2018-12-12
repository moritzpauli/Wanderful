// Fill out your copyright notice in the Description page of Project Settings.

#include "TextWidget.h"

FText UTextWidget::GetDisplayText() const
{
	return DisplayText;
}

void UTextWidget::SetDisplayText(const FText & NewDisplayText)
{
	DisplayText = NewDisplayText;
}
