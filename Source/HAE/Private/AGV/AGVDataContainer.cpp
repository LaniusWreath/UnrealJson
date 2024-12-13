// Fill out your copyright notice in the Description page of Project Settings.


#include "AGV/AGVDataContainer.h"

UAGVDataContainer::UAGVDataContainer()
{
    AGVData = FAGVData();
}

UAGVDataContainer* UAGVDataContainer::SetAGVData(const FAGVData& InputData)
{
    AGVData = InputData;
    return this;
}


