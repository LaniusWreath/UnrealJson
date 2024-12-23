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
 * Reflection 막아놨으니, 함수들 블루프린트에서 사용하고 싶으면 상속시켜 새로 클래스 만들 것
 * 
 */

class UJCMDataContainer;

DECLARE_DELEGATE_OneParam(FOnJsonObjectReceivedDelegate, const TSharedPtr<FJsonObject>);
DECLARE_DELEGATE(FOnJsonStringReceivedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDynamicRequestDelegate);

UCLASS(Blueprintable)
class HAE_API USFCHttpManager : public UObject
{
	GENERATED_BODY()

private:

protected:
	FString ResultResponseString;
	TSharedPtr<FJsonObject> ParsedJsonData;

	// Main Request Function
	
	// Request work delegate binding functions
	virtual void OnResponseReceivedWithString(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful); // with string
	virtual void OnResponseReceivedWithPtr(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful); // with objectptr

	// If you want to adjust custom parsing function, override this
	virtual void ExecuteCustomParseFucntion(TSharedPtr<FJsonObject> OriginJsonObject);

	// Json Object Response -> String to Json Object
	virtual TSharedPtr<FJsonObject> ParseRequestBody(TSharedPtr<FJsonObject> RequestBody);

	// Commonly Usable Parsing Function
	static TMap<FString, FString> ParseJsonStringToMap(const FString& JsonString);
	static TArray<FString> ParseStringToStringArray(const FString& ArrayString);
	static TArray<float> ParseStringToFloatArray(const FString& ArrayString);
	static TMap<FString, FString> ParseJsonObjToMap(const TSharedPtr<FJsonObject> OriginJsonObject);
	static FString ExtractDataFieldFromJsonString(const FString& JsonString);

public:
	// Delegate for Alarming Request Done, Data Ready
	FOnJsonObjectReceivedDelegate OnParsedJsonObjectPtrReady;
	FOnJsonStringReceivedDelegate OnRequestedJsonStringReady;

	// RequestFunction only URL
	virtual void MakeGetRequest(const FString& Url, const bool GetResultWithFString = true);

	// Reqeust Function URL with Header
	virtual void MakeGetRequestWithHeader(const FString& Url, const TMap<FString, FString>& Headers, 
		const TMap<FString, FString>& Parameters, const bool GetResultWithFString = true);

	// Blueprint Callable Delegate 
	UPROPERTY(BlueprintAssignable, Category = "SFC")
	FOnDynamicRequestDelegate OnDynamicRequestingEvent;

	// Return Serialized JsonString member
	UFUNCTION(BlueprintCallable, Category = "SFC")
	const FString& GetResultResponseString()
	{
		return ResultResponseString;
	}
	
	// Return JsonObjectPtr member
	const TSharedPtr<FJsonObject> GetJsonObject(){
		return ParsedJsonData;
	};

};

