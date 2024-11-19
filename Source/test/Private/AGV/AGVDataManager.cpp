// Fill out your copyright notice in the Description page of Project Settings.

#include "AGV/AGVDataManager.h"
#include "AGV/AGVDataContainer.h"
#include "JsonUtilities.h"

// 구조체 입력받아 컨테이너 인스턴싱
UAGVDataContainer* UAGVDataManager::CreateDataContainer(UObject* Outer, const FAGVData& InputData)
{
	UAGVDataContainer* NewContainer = NewObject<UAGVDataContainer>(Outer);
	if (NewContainer)
	{
		NewContainer->SetAGVData(InputData);
	}
	return NewContainer;
}

// Json 오브젝트 전달받아 구조체로 변환
FAGVData UAGVDataManager::CreateDataContainer(const TSharedPtr<FJsonObject> OriginObject)
{	
	FAGVData NewData = FAGVData();
	if (!OriginObject)
	{
		UE_LOG(LogTemp, Error, TEXT("AGVDataManager : Creating CreateDataContainer Failed"));
	}

	if (!FJsonObjectConverter::JsonObjectToUStruct(OriginObject.ToSharedRef(), &NewData))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to convert JSON to struct"));
	}

	return NewData;
}
