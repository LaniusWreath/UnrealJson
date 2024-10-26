// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpModule.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "HTTPRequestManager.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnJsonDataReadyDelegate, TSharedPtr<FJsonObject>);

UCLASS(Blueprintable)
class TEST_API UHTTPRequestManager : public UObject
{
	GENERATED_BODY()
	
public:

	// Call HTTP Get Request
	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void MakeGetRequest(const FString& Url);

	// Return Serialized JsonString
	UFUNCTION(BlueprintCallable, Category = "HTTP")
	const FString& GetResultResponseString()
	{
		return ResultResponseString;
	}

	// Return Deseiralized JsonObject
	const TSharedPtr<FJsonObject> GetJsonData()
	{
		return JsonData;
	}

	// Delegate for Alarming Request Done, Data Ready
	FOnJsonDataReadyDelegate OnJsonDataReady;

private:
	// HTTP Processing
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UPROPERTY()
	FString ResultResponseString;

	TSharedPtr<FJsonObject> JsonData;
};
