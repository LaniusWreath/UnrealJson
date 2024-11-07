// Fill out your copyright notice in the Description page of Project Settings.

#include "Datas/JCMCore.h"
#include "Datas/JCMDataManager.h"
#include "Datas/JCMHttpHandler.h"

UJCMCore* UJCMCore::JCMCoreInstance = nullptr;

// RequestHandler를 싱글톤으로 매니저처럼 사용
UJCMHttpHandler* UJCMCore::GetJCMRequestManager()
{
	if (!RequestManagerInstance)
	{
		RequestManagerInstance = NewObject<UJCMHttpHandler>(this);
	}
	return RequestManagerInstance;
}

UJCMCore* UJCMCore::GetJCMCore()
{
	// SingletonInstance가 없으면 생성하고 초기화
	if (!JCMCoreInstance)
	{
		JCMCoreInstance = NewObject<UJCMCore>();
		JCMCoreInstance->AddToRoot();  // GC 방지
	}
	return JCMCoreInstance;
}

UJCMDataManager* UJCMCore::GetJCMDataManager()
{
	// DataManager 인스턴스 생성 및 초기화
	if (!DataManagerInstance)
	{
		DataManagerInstance = NewObject<UJCMDataManager>(this);
	}
	return DataManagerInstance;
}

void UJCMCore::Destroy()
{
	if (JCMCoreInstance)
	{
		JCMCoreInstance->RemoveFromRoot();  // GC 방지 해제
		JCMCoreInstance = nullptr;
	}
}
