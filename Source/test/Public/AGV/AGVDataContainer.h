// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AGV/AGVDataTypes.h"
#include "AGVDataContainer.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TEST_API UAGVDataContainer : public UObject
{
	GENERATED_BODY()
	
private:
	FAGVData AGVData;

public:
	UFUNCTION(BlueprintCallable, Category = "AGV")
	UAGVDataContainer* SetAGVData(const FAGVData& InputData);

	UFUNCTION(BlueprintCallable, Category = "AGV")
	const FAGVData& GetAGVData() const
	{
		return AGVData;
	}
};
