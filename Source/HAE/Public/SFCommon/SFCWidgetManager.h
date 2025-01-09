// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SFCWidgetManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintAble)
class HAE_API USFCWidgetManager : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "SFC")
	UUserWidget* CreateWidgetFromClass(TSubclassOf<UUserWidget> WidetClass, FName InWidgetName, APlayerController* Owner);

	UFUNCTION(BlueprintCallable, Category = "SFC")
	void ShowWidget(FName WidgetName);

	UFUNCTION(BlueprintCallable, Category = "SFC")
	void HideWidget(FName WidgetName);

	UFUNCTION(BlueprintCallable, Category = "SFC")
	void ClearWidgetMap();

	UFUNCTION(BlueprintCallable, Category = "SFC")
	const UUserWidget* GetWidgetFromClass(FName InWidgetName) const;

	UFUNCTION(BlueprintCallable, Category = "SFC")
	static USFCWidgetManager* CreateWidgetManagerInstance(UObject* Outer, TSubclassOf<USFCWidgetManager> ManagerClass);

private:
	// Cached Widgets
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "JCM", meta = (AllowPrivateAccess = true))
	TMap<FName, UUserWidget*> WidgetMap;
};
