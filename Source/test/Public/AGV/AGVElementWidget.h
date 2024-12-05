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
<<<<<<< HEAD
	
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

=======

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV")
	TArray<float> TempValueArray;
	
	UFUNCTION(BlueprintCallable, Category = "AGV")
	void UpdateData(const TArray<float>& FloatData);

	UFUNCTION(BlueprintCallable, Category = "AGV")
	void WidgetRefresh();

>>>>>>> 1ca4cc6f10e5cea2b4d30e2f3d75ebd17acc6e94
};
