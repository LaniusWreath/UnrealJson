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
		UE_LOG(LogTemp, Warning, TEXT("No JCM Core Instance. Please Initialize Core First"));
	}
	return RequestManagerInstance;
}

UJCMCore* UJCMCore::GetJCMCore()
{
	if (JCMCoreInstance)
	{
		return JCMCoreInstance;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No JCM Core Instance. Please Initialize Core First"));
		return nullptr;
	}
}

UJCMCore* UJCMCore::InitializeJCMCore()
{
	// SingletonInstance가 없으면 생성하고 초기화
	if (!JCMCoreInstance)
	{
		JCMCoreInstance = NewObject<UJCMCore>();
		JCMCoreInstance->AddToRoot();  // GC 방지

		JCMCoreInstance->DataManagerInstance = NewObject<UJCMDataManager>();
		JCMCoreInstance->RequestManagerInstance = NewObject<UJCMHttpHandler>();
	}
	return JCMCoreInstance;
}

UJCMDataManager* UJCMCore::GetJCMDataManager()
{
	// DataManager 인스턴스 생성 및 초기화
	if (!DataManagerInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("No JCM Core Instance. Please Initialize Core First"));
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
