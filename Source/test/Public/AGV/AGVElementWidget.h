// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AGVElementWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UAGVElementWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV")
	TArray<float> TempValueArray;
	
	UFUNCTION(BlueprintCallable, Category = "AGV")
	void UpdateData(const TArray<float>& FloatData);

	UFUNCTION(BlueprintCallable, Category = "AGV")
	void WidgetRefresh();

};
