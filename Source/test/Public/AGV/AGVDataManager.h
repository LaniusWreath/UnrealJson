// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AGVDataTypes.h"
#include "AGVDataManager.generated.h"

class UAGVDataContainer;
/**
 * 
 */
UCLASS()
class TEST_API UAGVDataManager : public UObject
{
	GENERATED_BODY()

public:

	// 구조체 입력받아 컨테이너 인스턴싱
	UFUNCTION(BlueprintCallable, Category = "AGV")
	static UAGVDataContainer* CreateDataContainer(UObject* Outer, const FAGVData& InputData);
	
	// Json 객체 입력받아 컨테이너 인스턴싱
	static FAGVData CreateDataContainer(const TSharedPtr<FJsonObject> OriginObject);
};
