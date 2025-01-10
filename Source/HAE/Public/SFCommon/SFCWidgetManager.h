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
	UFUNCTION(BlueprintCallable, Category = "SFC|Widget")
	static USFCWidgetManager* CreateWidgetManagerInstance(UObject* Outer, TSubclassOf<USFCWidgetManager> ManagerClass);

	UFUNCTION(BlueprintCallable, Category = "SFC|Widget")
	UUserWidget* CreateWidgetFromClass(TSubclassOf<UUserWidget> WidetClass, FName InWidgetName, APlayerController* Owner);

	UFUNCTION(BlueprintCallable, Category = "SFC|Widget")
	void ShowWidget(FName WidgetName);

	UFUNCTION(BlueprintCallable, Category = "SFC|Widget")
	void HideWidget(FName WidgetName);

	UFUNCTION(BlueprintCallable, Category = "SFC|Widget")
	void ClearWidgetMap();

	UFUNCTION(BlueprintCallable, Category = "SFC|Widget")
	const UUserWidget* GetWidgetRef(FName InWidgetName) const;

	UFUNCTION(BlueprintCallable, Category = "SFC|Widget")
	void SetPlayerControllerRef(APlayerController* InPlayerController);
	
private:

	// Player Controller Ref
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SFC|Widget", meta = (AllowPrivateAccess = true))
	APlayerController* PlayerControllerRef;

	// Cached Widgets
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SFC|Widget", meta = (AllowPrivateAccess = true))
	TMap<FName, UUserWidget*> WidgetMap;
};
