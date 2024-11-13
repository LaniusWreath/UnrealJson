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

private:
	static UJCMCore* JCMCoreInstance;

	UPROPERTY()
	UJCMDataManager* DataManagerInstance;

	UPROPERTY()
	UJCMHttpHandler* RequestManagerInstance;

	UJCMDataManager* GetJCMDataManager();

	UJCMHttpHandler* GetJCMRequestManager();

public:
	// Get Jcm Core Instance
	UFUNCTION(BlueprintCallable, Category = "Chart")
	static UJCMCore* GetJCMCore();

	// Initialize JCM Core
	UFUNCTION(BlueprintCallable, Category = "Chart")
	static const UJCMCore* InitializeJCMCore();
	
	// Deleting JCM Core 
	UFUNCTION(BlueprintCallable, Category = "Chart")
	static void JCMCoreDestroy();

	// Get JCM HttpManager Reference
	UFUNCTION(BlueprintCallable, Category = "Chart")
	UJCMHttpHandler* GetHttpRequestManager() const { return RequestManagerInstance; }

	// Get JCM Data Containing Manager Reference
	UFUNCTION(BlueprintCallable, Category = "Chart")
	UJCMDataManager* GetDataManager() const { return DataManagerInstance; }

};
