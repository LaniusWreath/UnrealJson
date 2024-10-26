// Fill out your copyright notice in the Description page of Project Settings.

#include "Datas/JBCMCore.h"
#include "Datas/DataManager.h"
#include "Datas/HTTPRequestManager.h"

UJBCMCore* UJBCMCore::JBCMCoreInstance = nullptr;

UJBCMCore* UJBCMCore::GetJBCMCore()
{
	// SingletonInstance가 없으면 생성하고 초기화
	if (!JBCMCoreInstance)
	{
		JBCMCoreInstance = NewObject<UJBCMCore>();
		JBCMCoreInstance->AddToRoot();  // GC 방지
		JBCMCoreInstance->InitializeManagers();
	}
	return JBCMCoreInstance;
}

void UJBCMCore::InitializeManagers()
{
	// 각 Manager 인스턴스 생성 및 초기화
	RequestManagerInstance = NewObject<UHTTPRequestManager>(this);
	DataManagerInstance = NewObject<UDataManager>(this);
}

void UJBCMCore::Destroy()
{
	if (JBCMCoreInstance)
	{
		JBCMCoreInstance->RemoveFromRoot();  // GC 방지 해제
		JBCMCoreInstance = nullptr;
	}
}
