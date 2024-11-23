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

UCLASS(Blueprintable)
class TEST_API USFCHttpManager : public UObject
{
	GENERATED_BODY()

private:

protected:
	FString ResultResponseString;
	TSharedPtr<FJsonObject> ParsedJsonData;

	// Main Request Function

	virtual void OnResponseReceivedWithString(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	virtual void OnResponseReceivedWithPtr(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	virtual void ExecuteCustomParseFucntion(TSharedPtr<FJsonObject> OriginJsonObject);

	// Json Object Response -> String to Json Object
	virtual TSharedPtr<FJsonObject> ParseRequestBody(TSharedPtr<FJsonObject> RequestBody);
	

	// Commonly Usable Parsing Function

	static TMap<FString, FString> ParseJsonStringToMap(const FString& JsonString);
	static TArray<FString> ParseStringToStringArray(const FString& ArrayString);
	static TArray<float> ParseStringToFloatArray(const FString& ArrayString);
	static TMap<FString, FString> ParseJsonObjToMap(const TSharedPtr<FJsonObject> OriginJsonObject);


public:
	// Delegate for Alarming Request Done, Data Ready
	FOnJsonDataReadyDelegate OnParsedJsonObjectPtrReady;
	FOneParamDelegate OnRequestedJsonStringReady;

	virtual void MakeGetRequest(const FString& Url, const bool GetResultWithFString = true);

	// Blueprint Callable Delegate 
	UPROPERTY(BlueprintAssignable, Category = "Chart")
	FOnDynamicRequestEvent OnRequestingProcessDone;

	// Return Serialized JsonString
	UFUNCTION(BlueprintCallable, Category = "HTTP")
	const FString& GetResultResponseString()
	{
		return ResultResponseString;
	}
	
	const TSharedPtr<FJsonObject> GetJsonObject(){
		return ParsedJsonData;
	};

};

