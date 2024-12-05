// Fill out your copyright notice in the Description page of Project Settings.

#include "Datas/JCMCore.h"
#include "Datas/JCMDataManager.h"
#include "Datas/JCMHttpHandler.h"
#include "Datas/JCMWidgetManager.h"
#include "Datas/JCMLog.h"


UJCMCore* UJCMCore::JCMCoreInstance = nullptr;

// Initializing JCMCore static
const UJCMCore* UJCMCore::InitializeJCMCore()
{
	if (!JCMCoreInstance)
	{
		JCMCoreInstance = NewObject<UJCMCore>();
		JCMCoreInstance->AddToRoot();  // GC 방지

		JCMCoreInstance->DataManagerInstance = NewObject<UJCMDataManager>();
		JCMCoreInstance->RequestManagerInstance = NewObject<UJCMHttpHandler>();
		JCMCoreInstance->WidgetManagerInstance = NewObject<UJCMWidgetManager>();
	}
	return JCMCoreInstance;
}

// Get JCMCore Instance
UJCMCore* UJCMCore::GetJCMCore()
{
	if (JCMCoreInstance)
	{
		return JCMCoreInstance;
	}
	else
	{
		UE_LOG(JCMlog, Warning, TEXT("No JCM Core Instance. Please Initialize Core First"));
		return nullptr;
	}
}

// RequestHandler를 싱글톤으로 매니저처럼 사용 : 소스용
UJCMHttpHandler* UJCMCore::GetJCMRequestManager()
{
	if (!RequestManagerInstance)
	{
		UE_LOG(JCMlog, Warning, TEXT("No JCM Core Instance. Please Initialize Core First"));
	}
	return RequestManagerInstance;
}

// Get JCMDataManager static Instance : 소스용
UJCMDataManager* UJCMCore::GetJCMDataManager()
{
	// DataManager 인스턴스 생성 및 초기화
	if (!DataManagerInstance)
	{
		UE_LOG(JCMlog, Warning, TEXT("No JCM Core Instance. Please Initialize Core First"));
	}
	return DataManagerInstance;
}

// 코어 인스턴스 삭제 및 GC 방지 해제
void UJCMCore::JCMCoreDestroy()
{
	if (JCMCoreInstance)
	{
		JCMCoreInstance->RemoveFromRoot();  // GC 방지 해제
		JCMCoreInstance = nullptr;
	}
}
