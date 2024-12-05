// Fill out your copyright notice in the Description page of Project Settings.

#include "AGV/AGVElementWidget.h"

void UAGVElementWidget::UpdateData(const TArray<float>& FloatData)
{
	TempValueArray = FloatData;
	WidgetRefresh();
}

void UAGVElementWidget::UpdateLabelData(const TArray<FString>& LabelData)
{
	TempLabelArray = LabelData;
	WidgetRefresh();
}

void UAGVElementWidget::WidgetRefresh()
{
}
