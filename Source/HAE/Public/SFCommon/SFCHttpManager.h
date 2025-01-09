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
 * Reflection ���Ƴ�����, �Լ��� �������Ʈ���� ����ϰ� ������ ��ӽ��� ���� Ŭ���� ���� ��
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
	virtual void CallCustomParseFucntion(TSharedPtr<FJsonObject> OriginJsonObject);

	// Json Object Response -> String to Json Object
	virtual TSharedPtr<FJsonObject> ParseRequestBody(TSharedPtr<FJsonObject> RequestBody);

	static TMap<FString, FString> ParseJsonObjToMap(const TSharedPtr<FJsonObject> OriginJsonObject);

public:
	// Delegate for Alarming Request Done, Data Ready
	FOnJsonObjectReceivedDelegate OnParsedJsonObjectPtrReady;
	FOnJsonStringReceivedDelegate OnRequestedJsonStringReady;

	// RequestFunction only URL
	UFUNCTION(BlueprintCallable, Category = "SFC")
	virtual void MakeGetRequest(const FString& Url, const bool GetResultWithFString = true);

	// Reqeust Function URL with Header
	UFUNCTION(BlueprintCallable, Category = "SFC")
	virtual void MakeGetRequestWithHeader(const FString& Url, const TMap<FString, FString>& Headers, 
		const TMap<FString, FString>& Parameters, const bool GetResultWithFString = true);

	UFUNCTION(BlueprintCallable, Category = "SFC")
	static USFCHttpManager* CreateHttpManagerInstance(UObject* Outer);

public:
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

