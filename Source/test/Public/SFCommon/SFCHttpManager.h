// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpModule.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "SFCHttpManager.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnJsonDataReadyDelegate, const TSharedPtr<FJsonObject>);
DECLARE_DELEGATE_OneParam(FOneParamDelegate, const bool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDynamicRequestEvent);

UCLASS()
class TEST_API USFCHttpManager : public UObject
{
	GENERATED_BODY()

private:

protected:
	FString ResultResponseString;
	TSharedPtr<FJsonObject> ParsedJsonData;

	virtual void MakeGetRequest(const FString& Url, const bool GetResultWithFString = true);

	virtual void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	virtual void OnResponseReceivedWithPtr(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:
	// Delegate for Alarming Request Done, Data Ready
	FOnJsonDataReadyDelegate OnParsedJsonObjectPtrReady;
	FOneParamDelegate OnRequestedJsonStringReady;

	// Blueprint Callable Delegate 
	UPROPERTY(BlueprintAssignable, Category = "Chart")
	FOnDynamicRequestEvent OnRequestingProcessDone;

	// Return Serialized JsonString
	UFUNCTION(BlueprintCallable, Category = "HTTP")
	const FString& GetResultResponseString()
	{
		return ResultResponseString;
	}

};

