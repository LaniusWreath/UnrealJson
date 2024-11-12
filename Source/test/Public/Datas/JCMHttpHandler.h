// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpModule.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "JCMHttpHandler.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnJsonDataReadyDelegate, const TSharedPtr<FJsonObject>);
DECLARE_DELEGATE_OneParam(FOneParamDelegate, const bool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDynamicRequestEvent);

UCLASS(Blueprintable)
class TEST_API UJCMHttpHandler : public UObject
{
	GENERATED_BODY()
	
public:

	// Delegate for Alarming Request Done, Data Ready
	//FOnJsonDataReadyDelegate OnJsonDataReady;
	FOnJsonDataReadyDelegate OnParsedJsonObjectPtrReady;
	FOneParamDelegate OnRequestedJsonStringReady;

	UPROPERTY(BlueprintAssignable, Category = "Chart")
	FOnDynamicRequestEvent OnRequestingProcessDone;

	// Call HTTP Get Request
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "GetResultWithFString"), Category = "Chart")
	void MakeGetRequest(const FString& Url, const bool GetResultWithFString = true);

	// Return Serialized JsonString
	UFUNCTION(BlueprintCallable, Category = "HTTP")
	const FString& GetResultResponseString()
	{
		return ResultResponseString;
	}

	// Return Deseiralized JsonObject
	const TSharedPtr<FJsonObject> GetJsonData()
	{
		return ParsedJsonData;
	}

	UFUNCTION(BlueprintCallable, Category = "Chart")
	static TMap<FString, FString> ParseJsonStringToMap(const FString& JsonString);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	static TArray<float> ParseStringToFloatArray(const FString& ArrayString);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	static TArray<FString> ParseStringToStringArray(const FString& ArrayString);

private:
	// HTTP Processing
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void OnResponseReceivedWithPtr(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FString ResultResponseString;

	TSharedPtr<FJsonObject> ParsedJsonData;
	
	TMap<FString, FString> JsonMap;

	// Custom JsonParsing Function.
	TSharedPtr<FJsonObject> ParseRequestBody(TSharedPtr<FJsonObject> RequestBody);

	void ExecuteCustomParseFucntion(TSharedPtr<FJsonObject> OriginJsonObject);

};
