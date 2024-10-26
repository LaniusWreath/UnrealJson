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
UCLASS()
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
	void InitializeManagers();

public:

	// Core Approching Function
	static UJBCMCore* GetJBCMCore();

	// Core Deleting Function
	static void Destroy();

	// Each Manager Getter Function
	UHTTPRequestManager* GetHttpRequestManager() const { return RequestManagerInstance; }
	UDataManager* GetDataManager() const { return DataManagerInstance; }

};
