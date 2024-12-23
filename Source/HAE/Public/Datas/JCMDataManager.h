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
class HAE_API UJCMDataManager : public UObject
{
	GENERATED_BODY()
	
private:
	// Instancing CSVHandler and Get JsonObject Ptr
	void LoadDataFromCSV(const FString& FilePath);

	// Instancing HTTPHandler and Get JsonObject Ptr
	void FetchDataFromHTTP(const FString& URL);

	TSharedPtr<FJsonObject> DeserializeJsonStringToJsonObject(const FString& JsonString);

	// JSON String Serialization Function
	FString SerializeJSONToString(const TSharedPtr<FJsonObject> JSONObject);

public:
	// Create JCMDataContainer Instance
	UFUNCTION(BlueprintCallable, Category = "JCM")
	static UJCMDataManager* CreateJCMDataManagerInstance(UObject* Outer);

	// JsonObject Ptr to Data Struct <String Header, UDataClasses* DataClassInstance>
	FDataInstancePair InstancingDataContainer(const TSharedPtr<FJsonObject> Data);

	// JsonObject Ptr to Data Struct <String Header, UDataClasses* DataClassInstance>
	FDataInstancePair InstancingDataContainerToOuter(UObject* Outer, const TSharedPtr<FJsonObject> Data);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	UJCMDataContainer* InstancingDataContainerFromJsonString(const FString& JsonBody);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	UJCMDataContainer* GetInstancedDataContainerFromJsonString(UObject* Outer, const FString& JsonBody);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	UJCMDataContainer* CreateEmptyDataContainer(EJCMChartTypes ChartType);
};

