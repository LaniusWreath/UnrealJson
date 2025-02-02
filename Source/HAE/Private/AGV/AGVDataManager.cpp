// Fill out your copyright notice in the Description page of Project Settings.

#include "AGV/AGVDataManager.h"
#include "JsonUtilities.h"
#include "AGV/AGVLog.h"
#include "AGV/AGVDataContainer.h"
#include "SFCommon/SFCHttpManager.h"
#include "SFCommon/SFCWebSocketManager.h"


//--------------------------------- Data Management --------------------------------------------

// 데이터 매니저 인스턴스 Get
UAGVDataManager* UAGVDataManager::CreateAGVDataManagerInstance(UObject* Outer)
{
	return NewObject<UAGVDataManager>(Outer, UAGVDataManager::StaticClass());
}

// 구조체 입력받아 컨테이너 인스턴싱
UAGVDataContainer* UAGVDataManager::InstancingDataContainer(UObject* Outer, const FAGVData& InputData)
{
	UAGVDataContainer* NewContainer = NewObject<UAGVDataContainer>(Outer);
	if (NewContainer)
	{
		NewContainer->SetAGVData(InputData);
	}
	return NewContainer;
}

// AGVData Json 오브젝트 전달받아 구조체로 변환
FAGVData UAGVDataManager::JsonObjectToAGVStruct(const TSharedPtr<FJsonObject> OriginObject)
{
	FAGVData NewData = FAGVData();
	if (!OriginObject)
	{
		UE_LOG(AGVlog, Error, TEXT("AGVDataManager : Creating CreateDataContainer Failed"));
	}

	if (!FJsonObjectConverter::JsonObjectToUStruct(OriginObject.ToSharedRef(), &NewData))
	{
		UE_LOG(AGVlog, Error, TEXT("Failed to convert JSON to struct"));
	}

	return NewData;
}

FAGVData UAGVDataManager::JsonStringToAGVStruct(const FString& OriginString)
{
	FAGVData NewData = FAGVData();
	if (OriginString.IsEmpty())
	{
		UE_LOG(AGVlog, Error, TEXT("AGVDataManager : Creating CreateDataContainer Failed"));
	}

	if (!FJsonObjectConverter::JsonObjectStringToUStruct<FAGVData>(OriginString, &NewData))
	{
		UE_LOG(AGVlog, Error, TEXT("Failed to convert JSON to struct"));
	}

	return NewData;
}

// 빈 AGV 데이터 컨테이너 생성
UAGVDataContainer* UAGVDataManager::CreateEmptyDataContainer(UObject* Outer)
{
	UAGVDataContainer* NewContainer = NewObject<UAGVDataContainer>(Outer);

	return NewContainer;
}

// 데이터 컨테이너의 AGVData 구조체 업데이트
UAGVDataContainer* UAGVDataManager::UpdateDataContainer(UAGVDataContainer* InContainer, const FAGVData& InData)
{
	if (!InContainer)
	{
		UE_LOG(AGVlog, Warning, TEXT("UpdateDataContainer: InDataContainer is invalid"));
		return nullptr;
	}
	InContainer->SetAGVData(InData);
	return InContainer;
}
