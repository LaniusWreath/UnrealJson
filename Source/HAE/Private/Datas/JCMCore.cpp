// Fill out your copyright notice in the Description page of Project Settings.

#include "Datas/JCMCore.h"
#include "Datas/JCMDataManager.h"
#include "Datas/JCMHttpHandler.h"
#include "Datas/JCMWidgetManager.h"
#include "Datas/JCMLog.h"


UJCMCore* UJCMCore::JCMCoreInstance = nullptr;

UJCMCore::UJCMCore()
{
	WidgetManagerType = nullptr;
	WidgetManagerInstance = nullptr;
}

// JCMCore �ν��Ͻ� ���� 
const UJCMCore* UJCMCore::InitializeJCMCore(UObject* Outer, TSubclassOf<UJCMCore> ManagerClass)
{
	// ���� ���Ұ�� �⺻
	if (!ManagerClass)
	{
		ManagerClass = UJCMCore::StaticClass();
	}
	// ������ ��� �������Ʈ Ŭ����
	JCMCoreInstance = NewObject<UJCMCore>(Outer, ManagerClass);

	if (!JCMCoreInstance)
	{
		UE_LOG(JCMlog, Error, TEXT("Failed instancing JCMCore"));
		return nullptr;
	}

	return JCMCoreInstance;
}

void UJCMCore::InitializeManagers()
{
	JCMCoreInstance->DataManagerInstance = NewObject<UJCMDataManager>();
	JCMCoreInstance->RequestManagerInstance = NewObject<UJCMHttpHandler>();

	//JCMCoreInstance->WidgetManagerInstance = NewObject<UJCMWidgetManager>();
	initializeWidgetManager();
}

// ���� �Ŵ��� �ν��Ͻ� ����
void UJCMCore::initializeWidgetManager()
{
	// �������Ʈ�󿡼� �ٸ� ���� �Ŵ����� �������� ���
	if (WidgetManagerType)
	{
		WidgetManagerInstance = NewObject<UJCMWidgetManager>(this, WidgetManagerType);
	}
	//�⺻ Ÿ��
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AClass is not set! Default UA instance will be created."));
		WidgetManagerInstance = NewObject<UJCMWidgetManager>(this); // �⺻ ���� ����
	}
}


// Get JCMCore Instance
const UJCMCore* UJCMCore::GetJCMCore()
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

// RequestHandler�� �̱������� �Ŵ���ó�� ��� : �ҽ���
UJCMHttpHandler* UJCMCore::GetJCMRequestManager()
{
	if (!RequestManagerInstance)
	{
		UE_LOG(JCMlog, Warning, TEXT("No JCM Core Instance. Please Initialize Core First"));
	}
	return RequestManagerInstance;
}

// Get JCMDataManager static Instance : �ҽ���
UJCMDataManager* UJCMCore::GetJCMDataManager()
{
	// DataManager �ν��Ͻ� ���� �� �ʱ�ȭ
	if (!DataManagerInstance)
	{
		UE_LOG(JCMlog, Warning, TEXT("No JCM Core Instance. Please Initialize Core First"));
	}
	return DataManagerInstance;
}

// �ھ� �ν��Ͻ� ���� �� GC ���� ����
void UJCMCore::JCMCoreDestroy()
{
	if (JCMCoreInstance)
	{
		JCMCoreInstance->RemoveFromRoot();  // GC ���� ����
		JCMCoreInstance = nullptr;
	}
}
