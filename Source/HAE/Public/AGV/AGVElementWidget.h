// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AGVElementWidget.generated.h"

/**
 * 
 */
UCLASS()
class HAE_API UAGVElementWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "SmartFactoryCommon")
	void UpdateData(const TArray<float>& FloatData);

	UFUNCTION(BlueprintCallable, Category = "SmartFactoryCommon")
	void UpdateLabelData(const TArray<FString>& LabelData);

	UFUNCTION(BlueprintCallable, Category = "SmartFactoryCommon")
	void WidgetRefresh();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SmartFactoryCommon", meta = (AllowPrivateAccess = "true"))
	TArray<float> TempValueArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SmartFactoryCommon", meta = (AllowPrivateAccess = "true"))
	TArray<FString> TempLabelArray;

};
