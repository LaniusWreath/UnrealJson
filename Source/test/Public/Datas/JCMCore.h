// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JCMCore.generated.h"

class UJCMDataManager;
class UJCMHttpHandler;

/**
 * 
 */
UCLASS(Blueprintable)
class TEST_API UJCMCore : public UObject
{
	GENERATED_BODY()

public:
	// Get Jcm Core Instance
	UFUNCTION(BlueprintCallable, Category = "JCM")
	static UJCMCore* GetJCMCore();

	// Initialize JCM Core
	UFUNCTION(BlueprintCallable, Category = "JCM")
	static const UJCMCore* InitializeJCMCore();
	
	// Deleting JCM Core 
	UFUNCTION(BlueprintCallable, Category = "JCM")
	static void JCMCoreDestroy();

	// Get JCM HttpManager Reference
	UFUNCTION(BlueprintCallable, Category = "JCM")
	UJCMHttpHandler* GetHttpRequestManager() const { return RequestManagerInstance; }

	// Get JCM Data Containing Manager Reference
	UFUNCTION(BlueprintCallable, Category = "JCM")
	UJCMDataManager* GetDataManager() const { return DataManagerInstance; }

	UFUNCTION(BlueprintCallable, Category = "JCM")
	const UJCMWidgetManager* GetWidgetManager() const { return WidgetManagerInstance; }

private:
	static UJCMCore* JCMCoreInstance;
	UJCMDataManager* GetJCMDataManager();
	UJCMHttpHandler* GetJCMRequestManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "JCM", meta = (AllowPrivateAccess = true))
	UJCMDataManager* DataManagerInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "JCM", meta = (AllowPrivateAccess = true))
	UJCMHttpHandler* RequestManagerInstance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "JCM", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UJCMWidgetManager> WidgetManagerInstance;

};
