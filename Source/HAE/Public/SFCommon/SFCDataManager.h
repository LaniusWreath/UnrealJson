// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SFCDataTypes.h"
#include "SFCDataManager.generated.h"

/**
 * 
 */

class USFCDataManager;

UCLASS(Blueprintable)
class HAE_API USFCDataManager : public UObject
{
	GENERATED_BODY()

public:

	//--------------------------------- DataManagement --------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "SFC")
	static USFCDataManager* CreateSFCDataManagerInstance(UObject* Outer);

	// Instancing GenericDataContainer from Struct input
	UFUNCTION(BlueprintCallable, Category = "SFC")
	USFCDataContainer* InstancingDataContainer(UObject* Outer, const FGenericData& InData);

	UFUNCTION(BlueprintCallable, Category = "SFC")
	USFCDataContainer* CreateEmptyDataContainer(UObject* Outer);

	UFUNCTION(BlueprintCallable, Category = "SFC")
	USFCDataContainer* UpdateDataContainer(USFCDataContainer* InContainer, const FGenericData& InData);

	UFUNCTION(BlueprintCallable, Category = "SFC")
	bool StringToSFCDataStruct(const FString& InString, FGenericData& InDataStruct);


	// ------------------------------ UtilityFunction ------------------------------------
	UFUNCTION(BlueprintCallable, Category = "SFC")
	static TMap<FString, FString> ParseJsonStringToMap(const FString& JsonString);

	UFUNCTION(BlueprintCallable, Category = "SFC")
	static TArray<FString> ParseStringToStringArray(const FString& ArrayString);

	UFUNCTION(BlueprintCallable, Category = "SFC")
	static TArray<float> ParseStringToFloatArray(const FString& ArrayString);

	UFUNCTION(BlueprintCallable, Category = "SFC")
	static FString ExtractDataFieldFromJsonString(const FString& JsonString);

	// ------------------------------ Debuging -------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "SFC")
	static void PrintStringMapMembers(const TMap<FString, FString>& InMap, const float Duration = 5, 
		const FColor TextColor = FColor::Green);
};
