// Fill out your copyright notice in the Description page of Project Settings.


#include "SFCommon/SFCWidgetManager.h"
#include "Blueprint/UserWidget.h"
#include "SFCommon/SFCLog.h"

// ���� �Ŵ����� ���, �������Ʈ ���� �Լ��� �ʿ��ϴ�, Ŭ���� ���� ������ �� �ְԲ� 
USFCWidgetManager* USFCWidgetManager::CreateWidgetManagerInstance(UObject* Outer, TSubclassOf<USFCWidgetManager> ManagerClass)
{
	// Ŭ���� ���� ���� ���
	if (!ManagerClass)
	{
		return NewObject<USFCWidgetManager>(Outer, USFCWidgetManager::StaticClass());
	}

	return NewObject<USFCWidgetManager>(Outer, ManagerClass);
}

UUserWidget* USFCWidgetManager::CreateWidgetFromClass(TSubclassOf<UUserWidget> WidetClass, FName InWidgetName, 
	APlayerController* Owner)
{
	if (WidgetMap.Contains(InWidgetName))
	{
		UE_LOG(SFClog, Warning, TEXT("Widet instanced"));
		return WidgetMap[InWidgetName];
	}

	if (!WidetClass)
	{
		UE_LOG(SFClog, Error, TEXT("Invalid Widget Class"));
		return nullptr;
	}

	// ���� �⺻ ������
	if (!Owner)
	{
		UE_LOG(SFClog, Error, TEXT("JCMWidgetManager : Owner is invalid"));
		return nullptr;
	}

	// ���� ����
	UUserWidget* NewWidget = CreateWidget<UUserWidget>(Owner, WidetClass);
	if (!NewWidget)
	{
		UE_LOG(SFClog, Error, TEXT("JCMWidgetManager : Creating Widget failed"));
		return nullptr;
	}

	// ���� �ʿ� ���� ���� ���� �߰�
	WidgetMap.Emplace(InWidgetName, NewWidget);
	return NewWidget;
}

void USFCWidgetManager::ShowWidget(FName WidgetName)
{
	if (WidgetMap.Contains(WidgetName))
	{
		UUserWidget* TargetWidget = WidgetMap[WidgetName];
		if (TargetWidget && !TargetWidget->IsInViewport())
		{
			TargetWidget->AddToViewport();
		}
	}
}

void USFCWidgetManager::HideWidget(FName WidgetName)
{
	if (WidgetMap.Contains(WidgetName))
	{
		UUserWidget* TargetWidget = WidgetMap[WidgetName];
		if (TargetWidget && TargetWidget->IsInViewport())
		{
			TargetWidget->RemoveFromViewport();
		}
	}
}

void USFCWidgetManager::ClearWidgetMap()
{
	WidgetMap.Empty();
}

const UUserWidget* USFCWidgetManager::GetWidgetFromClass(FName InWidgetName) const
{
	if (!WidgetMap.Contains(InWidgetName))
	{
		UE_LOG(SFClog, Error, TEXT("No such widget"));
		return nullptr;
	}

	return WidgetMap[InWidgetName];
}

