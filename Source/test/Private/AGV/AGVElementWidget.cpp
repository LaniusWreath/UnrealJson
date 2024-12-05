// Fill out your copyright notice in the Description page of Project Settings.

#include "AGV/AGVElementWidget.h"

void UAGVElementWidget::UpdateData(const TArray<float>& FloatData)
{
	TempValueArray = FloatData;
	WidgetRefresh();
}

<<<<<<< HEAD
void UAGVElementWidget::UpdateLabelData(const TArray<FString>& LabelData)
{
	TempLabelArray = LabelData;
	WidgetRefresh();
}

=======
>>>>>>> 1ca4cc6f10e5cea2b4d30e2f3d75ebd17acc6e94
void UAGVElementWidget::WidgetRefresh()
{
}
