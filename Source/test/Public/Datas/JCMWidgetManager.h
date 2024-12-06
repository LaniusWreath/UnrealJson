// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JCMWidgetManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintAble)
class TEST_API UJCMWidgetManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "JCM")
	UUserWidget* CreateWidgetFromClass(TSubclassOf<UUserWidget> WidetClass, FName WidgetName, APlayerController* Owner);

	UFUNCTION(BlueprintCallable, Category = "JCM")
	void ShowWidget(FName WidgetName);

	UFUNCTION(BlueprintCallable, Category = "JCM")
	void HideWidget(FName WidgetName);

private:
	// Cached Widgets
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "JCM", meta = (AllowPrivateAccess = true))
	TMap<FName, UUserWidget*> WidgetMap;

};
