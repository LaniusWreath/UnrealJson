// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/JCMWidgetManager.h"
#include "Blueprint/UserWidget.h"
#include "Datas/JCMLog.h"

// ���� Ŭ���� �̸����� ĳ��
UUserWidget* UJCMWidgetManager::CreateWidgetFromClass(TSubclassOf<UUserWidget> WidetClass, FName InWidgetName, 
	APlayerController* Owner)
{
	if (WidgetMap.Contains(InWidgetName))
	{
		UE_LOG(JCMlog, Warning, TEXT("Widet instanced"));
		return WidgetMap[InWidgetName];
	}

	if (!WidetClass)
	{
		UE_LOG(JCMlog, Error, TEXT("Invalid Widget Class"));
		return nullptr;
	}

	// ���� �⺻ ������
	if (!Owner)
	{
		UE_LOG(JCMlog, Error, TEXT("JCMWidgetManager : Owner is invalid"));
		return nullptr;
	}

	// ���� ����
	UUserWidget* NewWidget = CreateWidget<UUserWidget>(Owner, WidetClass);
	if (!NewWidget)
	{
		UE_LOG(JCMlog, Error, TEXT("JCMWidgetManager : Creating Widget failed"));
		return nullptr;
	}

	// ���� �ʿ� ���� ���� ���� �߰�
	WidgetMap.Emplace(InWidgetName, NewWidget);
	return NewWidget;
}

// ���� ����Ʈ�� �߰�
void UJCMWidgetManager::ShowWidget(FName WidgetName)
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

// ���� ����Ʈ���� ����
void UJCMWidgetManager::HideWidget(FName WidgetName)
{
	if (WidgetMap.Contains(WidgetName))
	{
		UUserWidget* TargetWidget = WidgetMap[WidgetName];
		if (TargetWidget && TargetWidget->IsInViewport())
		{
			TargetWidget->RemoveFromParent();
		}
	}
}

void UJCMWidgetManager::ClearWidgetMap()
{
	WidgetMap.Empty();
}

const UUserWidget* UJCMWidgetManager::GetWidgetFromClass(FName InWidgetName) const
{
	if (!WidgetMap.Contains(InWidgetName))
	{
		UE_LOG(JCMlog, Error, TEXT("No such widget"));
		return nullptr;
	}

	return WidgetMap[InWidgetName];
}
