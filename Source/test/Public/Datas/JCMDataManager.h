// Fill out your copyright notice in the Description page of Project Settings

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SharedPointer.h"
#include "JCMDataTypes.h"
#include "JCMDataManager.generated.h"

class UJCMJsonHandler;
class UJCMDataContainer;
class UJCMDataContainerBar;

// Struct With String Header And Data
USTRUCT(BlueprintType)
struct FDataInstancePair
{
	GENERATED_BODY()

	UPROPERTY()
	bool IsValid;

	UPROPERTY()
	UJCMDataContainer* DataInstance;

	FDataInstancePair() : IsValid(false), DataInstance(nullptr) {}
};

// Manage Datas Class and Control those Member Function
UCLASS(Blueprintable, BlueprintType)
class TEST_API UJCMDataManager : public UObject
{
	GENERATED_BODY()

private:

	// DataHander Instances
	UJCMJsonHandler* JSONHandlerInstance;

	// JSON String Serialization Function
	FString SerializeJSONToString(const TSharedPtr<FJsonObject> JSONObject);
	FString DataString;

	// Instancing JsonHandler and Get JsonObject Ptr
	TSharedPtr<FJsonObject> LoadDataFromJSON(const FString& FilePath);

	// Instancing CSVHandler and Get JsonObject Ptr
	void LoadDataFromCSV(const FString& FilePath);

	// Instancing HTTPHandler and Get JsonObject Ptr
	void FetchDataFromHTTP(const FString& URL);

	TSharedPtr<FJsonObject> DeserializeJsonStringToJsonObject(const FString& JsonString);


public:
	// JsonObject Ptr to Data Struct <String Header, UDataClasses* DataClassInstance>
	FDataInstancePair InstancingDataContainer(const TSharedPtr<FJsonObject> Data);

	// Routine Function for Controlling Json Reading to Processing Functions
	UFUNCTION(BlueprintCallable, Category = "Chart")
	UJCMDataContainer* InstancingDataContainerFromLocalJson(const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	UJCMDataContainer* InstancingDataContainerFromJsonString(const FString& JsonBody);

	// Getter Serialized JSON String Data
	UFUNCTION(BlueprintCallable, Category = "Data")
	const FString& GetJSONStringData() const;

	UFUNCTION(BlueprintCallable, Category = "Chart")
	UJCMDataContainer* CreateEmptyDataContainer(EJCMChartTypes ChartType);
};

