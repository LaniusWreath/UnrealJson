// Fill out your copyright notice in the Description page of Project Settings.

#include "AGV/AGVMainWidget.h"
#include "AGV/AGVDataContainer.h"
#include "AGV/AGVElementWidget.h"
#include "Blueprint/UserWidget.h"


void UAGVMainWidget::UpdateAGVDataContainer(UAGVDataContainer* DataContainer)
{
	if (DataContainer) {
		AGVData = DataContainer->GetAGVData();
	}
}

void UAGVMainWidget::UpdateAGVDataWidgets(const FAGVData& SourceDataStruct)
{
	UI_AGV_Position->UpdateData({ SourceDataStruct.Position.X, SourceDataStruct.Position.Y, SourceDataStruct.Position.Z });
	UI_AGV_Rotation->UpdateData({ SourceDataStruct.Position.Yaw, SourceDataStruct.Position.Pitch, SourceDataStruct.Position.Roll });
}



