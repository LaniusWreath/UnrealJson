// Fill out your copyright notice in the Description page of Project Settings.

#include "AGV/AGVDataManager.h"
#include "JsonUtilities.h"
#include "AGV/AGVLog.h"
#include "AGV/AGVDataContainer.h"
#include "SFCommon/SFCHttpManager.h"


// Static
UAGVDataManager* UAGVDataManager::AGVDataManagerInstance = nullptr;

const UAGVDataManager* UAGVDataManager::GetAGVDataManager()
{
	if (!AGVDataManagerInstance)
	{
		AGVDataManagerInstance = NewObject<UAGVDataManager>();
		AGVDataManagerInstance->AddToRoot();  // GC 방지
	}
	return AGVDataManagerInstance;
}

// Json 오브젝트 전달받아 구조체로 변환
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

//---------------------------------

// Public Instance 
USFCHttpManager* UAGVDataManager::InitializeHttpHandler()
{
	if (!HttpHandler)
	{
		HttpHandler = NewObject<USFCHttpManager>(this);
		HttpHandler->OnParsedJsonObjectPtrReady.BindUObject(this, &UAGVDataManager::SetJsonObject);
	}
	return HttpHandler;
}

USFCHttpManager* UAGVDataManager::GetHttpHandler()
{
	if (!HttpHandler)
	{
		return nullptr;
	}
	return HttpHandler;
}

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

UAGVDataContainer* UAGVDataManager::CreateEmptyDataContainer(UObject* Outer)
{
	UAGVDataContainer* NewContainer = NewObject<UAGVDataContainer>(Outer);

	return NewContainer;
}

// 기존 데이터 콘테이너 객체의 데이터 직접 입력하여 업데이트
UAGVDataContainer* UAGVDataManager::UpdateContainerwithLastData(UAGVDataContainer* TargetContainer)
{
	if (!TargetContainer)
	{
		UE_LOG(AGVlog, Error, TEXT("Faild to find target AGVDataContainer"));
		return nullptr;
	}
	if (!TempJsonObject)
	{
		UE_LOG(AGVlog, Warning, TEXT("No JsonObject left"));
	}
	TargetContainer->SetAGVData(JsonObjectToAGVStruct(TempJsonObject));

	return TargetContainer;
}

void UAGVDataManager::RequestJsonObject(const FString& URL)
{
	if(!HttpHandler)
	{
		UE_LOG(AGVlog, Error, TEXT("Failed to find HttpHandler"))
		return;
	}
 	HttpHandler->MakeGetRequest(URL, false);
}

void UAGVDataManager::SetJsonObject(const TSharedPtr<FJsonObject> OriginObject)
{
	if (!OriginObject)
	{
		UE_LOG(AGVlog, Error, TEXT("Failed to find Origin Json Object"));
		return;
	}
	TempJsonObject = OriginObject;
}
