// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGVDataTypes.h"
#include "Blueprint/UserWidget.h"
#include "AGVMainWidget.generated.h"

class UAGVDataContainer;
/**
 * 
 */
UCLASS()
class TEST_API UAGVMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	//virtual void NativeConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AGV")
	FAGVData AGVData;

	UFUNCTION(BlueprintCallable, Category = "AGV")
	void UpdateAGVDataContainer(UAGVDataContainer* DataContainer);

	UFUNCTION(BlueprintCallable, Category = "AGV")
	void UpdateAGVDataWidgets(FAGVData& SourceDataStruct);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UAGVElementWidget* UI_AGV_Position;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UAGVElementWidget* UI_AGV_Rotation;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UAGVElementWidget* UI_AGV_Quaternion;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UAGVElementWidget* UI_AGV_Encoder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UAGVElementWidget* UI_AGV_Odometry;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UAGVElementWidget* UI_AGV_Wheel;



};
