// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/JCMWidgetManager.h"
#include "Blueprint/UserWidget.h"
#include "Datas/JCMLog.h"

// À§Á¬ Å¬·¡½º ÀÌ¸§À¸·Î Ä³½Ì
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

	// À§Á¬ ±âº» ¼ÒÀ¯ÀÚ
	if (!Owner)
	{
		UE_LOG(JCMlog, Error, TEXT("JCMWidgetManager : Owner is invalid"));
		return nullptr;
	}

	// À§Á¬ »ý¼º
	UUserWidget* NewWidget = CreateWidget<UUserWidget>(Owner, WidetClass);
	if (!NewWidget)
	{
		UE_LOG(JCMlog, Error, TEXT("JCMWidgetManager : Creating Widget failed"));
		return nullptr;
	}

	// À§Á¬ ¸Ê¿¡ »õ·Î ¸¸µç À§Á¬ Ãß°¡
	WidgetMap.Emplace(InWidgetName, NewWidget);
	return NewWidget;
}

// À§Á¬ ºäÆ÷Æ®¿¡ Ãß°¡
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

// À§Á¬ ºäÆ÷Æ®¿¡¼­ »èÁ¦
void UJCMWidgetManager::HideWidget(FName WidgetName)
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
