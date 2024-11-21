// Fill out your copyright notice in the Description page of Project Settings.

#include "JsonUtilities.h"
#include "AGV/AGVDataContainer.h"
#include "SFCommon/SFCHttpManager.h"
#include "AGV/AGVDataManager.h"

UAGVDataManager* UAGVDataManager::AGVDataManagerInstance = nullptr;

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
	if (HttpHandler)
		return HttpHandler;
	else
		return nullptr;
}

UAGVDataManager* UAGVDataManager::GetAGVDataManager()
{
	return AGVDataManagerInstance;
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

// 기존 데이터 콘테이너 객체의 데이터 직접 입력하여 업데이트
UAGVDataContainer* UAGVDataManager::UpdateContainerwithLastData(UAGVDataContainer* TargetContainer)
{
	if (!TargetContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("Faild to find target AGVDataContainer"));
		return nullptr;
	}
	if (!TempJsonObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("No JsonObject left"));
	}
	TargetContainer->SetAGVData(JsonObjectToAGVStruct(TempJsonObject));

	return TargetContainer;
}

// Json 오브젝트 전달받아 구조체로 변환
FAGVData UAGVDataManager::JsonObjectToAGVStruct(const TSharedPtr<FJsonObject> OriginObject)
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

void UAGVDataManager::RequestJsonObject(const FString& URL)
{
	if(!HttpHandler)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find HttpHandler"))
		return;
	}
//	HttpHandler->MakeGetRequest(URL, false);
}

void UAGVDataManager::SetJsonObject(const TSharedPtr<FJsonObject> OriginObject)
{
	if (!OriginObject)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find Origin Json Object"));
		return;
	}
	TempJsonObject = OriginObject;
}
