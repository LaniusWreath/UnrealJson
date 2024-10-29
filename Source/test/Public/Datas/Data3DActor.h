// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTypes.h"
#include "Data3DActor.generated.h"

class UDataManager;
class UDataClasses;
class UShapeChartClass;
class UChartGenerator;
class UTextRenderComponent;
class UHTTPRequestManager;

UCLASS(Abstract)
class TEST_API AData3DActor : public AActor
{
	GENERATED_BODY()

public:	
	AData3DActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetJsonObject(const TSharedPtr<FJsonObject> JsonData);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chart")
	FString CurrentChartType;

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void CallJsonObject(const FString& URL);

	// Initializing Data Manager Getting from Game Instance
	UFUNCTION()
	void InitilizeManagers();

	// Visualization Chart Title
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponent_chartTitle;
	
	// Http Request URL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart")
	FString HttpRequestURL;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Pure Virtual Setter Data Class Instance. Getting from DataManager's ChartDataClassInstanceArray.
	/*UFUNCTION(BlueprintCallable, Category = "Chart")
	virtual UDataClasses* SetDataClassInstance() PURE_VIRTUAL(UDataFetcherBase::FetchData, ;);*/

	// Pure Virtual Routine for Generate Chart
	virtual void GenerateChartRoutine() PURE_VIRTUAL(UDataFetcherBase::FetchData, ;);

	// DataManager Reference
	UPROPERTY()
	UDataManager* DataManagerInstance;

	// Data Class Instance
	UPROPERTY()
	UDataClasses* DataClassInstance;

	UPROPERTY()
	UHTTPRequestManager* RequestManagerInstance;

	UPROPERTY()
	USceneComponent* RootSceneComponent;

};

class UBarGenerator;
class ABarBaseActor;

UCLASS(Blueprintable)
class AData3DActorBar : public AData3DActor
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void SetChartDefaultTexts();

protected:
	//virtual UDataClasses* SetDataClassInstance() override;
	UFUNCTION(BlueprintCallable, Category = "Chart")
	virtual void GenerateChartRoutine() override;

public:
	AData3DActorBar();

	// Select Bar Blueprint Actor Source to Generate
	UPROPERTY(EditAnywhere, BlueprintReadOnly ,Category = "Chart")
	TSubclassOf<ABarBaseActor> BarBaseActorBPClass;

	// Controler Component for Generating 3D Bar Chart 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Chart")
	UBarGenerator* BarGeneratorComponent;

	// Visualization Chart Xaxis Name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponent_chartXaxisName;

	// Visualization Chart Yaxis Name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponent_chartYaxisName;

	// json을 받아와 DataClass라는 상위 데이터 컨테이너 클래스에 공통으로 저장하고있음.
	// 또한 이 공통 데이터는 공통 액터 추상클래스에 속해있음
	// 블루프린트에서는 액터가 자식 클래스로 구체화되어있으므로, 데이터 컨테이너 클래스를 호출 할 때에도 액터 형식에 맞게
	// 캐스팅해주는 과정이 필요함. 다른 자식 3DActor 클래스에도 구성해줄 것.
	// Get Json Data Container Class
	UFUNCTION(BlueprintCallable, Category = "Chart")
	const UShapeChartClass* GetData() const
	{
		UShapeChartClass* CastedDataClassInstance = Cast<UShapeChartClass>(DataClassInstance);
		return CastedDataClassInstance;
	}
};