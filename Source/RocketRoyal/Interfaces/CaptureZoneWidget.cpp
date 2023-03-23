// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureZoneWidget.h"
#include "Components/ProgressBar.h"

void UCaptureZoneWidget::SetBarProgress(float Percent) const
{
	Bar->SetPercent(Percent);
}

float UCaptureZoneWidget::GetBarProgress() const
{
	return Bar->Percent;
}
