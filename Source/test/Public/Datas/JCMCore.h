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
	// Initializing Each Managers
	UFUNCTION(BlueprintCallable, Category = "Chart")
	static UJCMCore* GetJCMCore();

	// Core Approching Function
	UFUNCTION(BlueprintCallable, Category = "Chart")
	static const UJCMCore* InitializeJCMCore();

	UFUNCTION(BlueprintCallable, Category = "Chart")
	// Core Deleting Function
	static void JCMCoreDestroy();

	// Each Manager Getter Function
	UFUNCTION(BlueprintCallable, Category = "Chart")
	UJCMHttpHandler* GetHttpRequestManager() const { return RequestManagerInstance; }

	UFUNCTION(BlueprintCallable, Category = "Chart")
	UJCMDataManager* GetDataManager() const { return DataManagerInstance; }

};
