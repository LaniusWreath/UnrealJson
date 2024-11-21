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

void UAGVMainWidget::UpdateAGVDataWidgets(FAGVData& SourceDataStruct)
{
	UI_AGV_Position->UpdateData({ SourceDataStruct.x, SourceDataStruct.y, SourceDataStruct.theta });
	UI_AGV_Rotation->UpdateData({ SourceDataStruct.yaw, SourceDataStruct.pitch, SourceDataStruct.roll });
	UI_AGV_Quaternion->UpdateData({ SourceDataStruct.orientation_x, SourceDataStruct.orientation_y, SourceDataStruct.orientation_z,
		SourceDataStruct.orientation_w});
	UI_AGV_Encoder->UpdateData({ float(SourceDataStruct.l_enc), float(SourceDataStruct.r_enc) });
	UI_AGV_Odometry->UpdateData({ SourceDataStruct.odo_l, SourceDataStruct.odo_r });
	UI_AGV_Wheel->UpdateData({ float(SourceDataStruct.gear_ratio), SourceDataStruct.wheel_separation,
		SourceDataStruct.wheel_radius });
}



