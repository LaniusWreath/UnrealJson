// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JBCMCore.generated.h"

class UDataManager;
class UHTTPRequestManager;

/**
 * 
 */
UCLASS(Blueprintable)
class TEST_API UJBCMCore : public UObject
{
	GENERATED_BODY()

private:

	static UJBCMCore* JBCMCoreInstance;

	UPROPERTY()
	UDataManager* DataManagerInstance;

	UPROPERTY()
	UHTTPRequestManager* RequestManagerInstance;

protected:
	// Initializing Each Managers
	UFUNCTION(BlueprintCallable, Category = "Chart")
	void InitializeManagers();

public:

	// Core Approching Function
	UFUNCTION(BlueprintCallable, Category = "Chart")
	static UJBCMCore* GetJBCMCore();

	UFUNCTION(BlueprintCallable, Category = "Chart")
	// Core Deleting Function
	static void Destroy();

	// Each Manager Getter Function
	UFUNCTION(BlueprintCallable, Category = "Chart")
	UHTTPRequestManager* GetHttpRequestManager() const { return RequestManagerInstance; }

	UFUNCTION(BlueprintCallable, Category = "Chart")
	UDataManager* GetDataManager() const { return DataManagerInstance; }

};
