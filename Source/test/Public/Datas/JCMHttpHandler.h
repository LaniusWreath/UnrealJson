// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SFCommon/SFCHttpManager.h"
#include "JCMHttpHandler.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class TEST_API UJCMHttpHandler : public USFCHttpManager
{
	GENERATED_BODY()

public:
	// Call HTTP Get Request
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "GetResultWithFString"), Category = "Chart")
	virtual void MakeGetRequest(const FString& Url, const bool GetResultWithFString = true) override;

	UFUNCTION(BlueprintCallable, Category = "Chart")
	static TMap<FString, FString> ParseJsonStringToMap(const FString& JsonString);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	static TArray<float> ParseStringToFloatArray(const FString& ArrayString);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	static TArray<FString> ParseStringToStringArray(const FString& ArrayString);

public:

private:

	// HTTP Processing
	void OnResponseReceivedWithString(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) override;

	void OnResponseReceivedWithPtr(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) override;

	// Custom JsonParsing Function.
	TSharedPtr<FJsonObject> ParseRequestBody(TSharedPtr<FJsonObject> RequestBody) override;

	void ExecuteCustomParseFucntion(TSharedPtr<FJsonObject> OriginJsonObject) override;

};