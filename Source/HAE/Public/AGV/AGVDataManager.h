// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AGVDataTypes.h"
#include "AGVDataManager.generated.h"

class USFCHttpManager;
class UAGVDataContainer;
class USFCWebSocketManager;
/**
 * 
 */
UCLASS(Blueprintable)
class HAE_API UAGVDataManager : public UObject
{
	GENERATED_BODY()

public:

	//--------------------------------- DataManagement --------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "AGV")
	static const UAGVDataManager* GetAGVDataManager();

	// Instancing AGVDataContainer from Struct input
	UFUNCTION(BlueprintCallable, Category = "AGV")
	UAGVDataContainer* InstancingDataContainer(UObject* Outer, const FAGVData& InData);

	UFUNCTION(BlueprintCallable, Category = "AGV")
	UAGVDataContainer* CreateEmptyDataContainer(UObject* Outer);

	UFUNCTION(BlueprintCallable, Category = "AGV")
	UAGVDataContainer* UpdateDataContainer(UAGVDataContainer* InContainer, const FAGVData& InData);

	// Get AGVDataStruct from jsonObject
	static FAGVData JsonObjectToAGVStruct(const TSharedPtr<FJsonObject> OriginObject);

	UFUNCTION(BlueprintCallable, Category = "AGV")
	static FAGVData JsonStringToAGVStruct(const FString& OriginString);

private:

	static UAGVDataManager* AGVDataManagerInstance;

};