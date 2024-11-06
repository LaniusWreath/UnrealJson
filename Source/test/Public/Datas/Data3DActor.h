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

UCLASS()
class TEST_API AData3DActor : public AActor
{
	GENERATED_BODY()

public:	
	AData3DActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetJsonObject(const TSharedPtr<FJsonObject> JsonData);

	void SetJsonString(const FString& JsonString);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chart")
	FString CurrentChartType;
	
	// Call Json Request Function, Result Data will be stored in this  
	UFUNCTION(BlueprintCallable, Category = "Chart")
	void RequestJsonObject(const FString& URL);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void RequestJsonString(const FString& URL);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	const UHTTPRequestManager* GetHttpRequestHandler() const
	{
		if (RequestManagerInstance){
			return RequestManagerInstance;
		}
		else
			return nullptr;
	}



	// Visualization Chart Title
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponent_chartTitle;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Chart")
	bool IsDataClassInstanceSet;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Initializing Data Manager Getting from Game Instance
	UFUNCTION()
	void InitializeDataManager();

	// Initializing Request Manager Instance : this instance have to be initialized in every CallJsonRoutine()
	UFUNCTION()
	void InitializeRequestManager();

	// Pure Virtual Routine for Generate Chart
	virtual void GenerateChartRoutine() PURE_VIRTUAL(UDataFetcherBase::FetchData, ;);

	// DataManager Reference
	UPROPERTY()
	UDataManager* DataManagerInstance;

	// Data Class Instance
	UPROPERTY(VisibleAnywhere, Category = "Chart")
	UDataClasses* DataClassInstance;

	UPROPERTY()
	UHTTPRequestManager* RequestManagerInstance;

	UPROPERTY()
	USceneComponent* RootSceneComponent;

	UPROPERTY()
	FString ResponsedJsonString;

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

	// On : Spawn Chart Mesh At Each Spline Point / Off : Spawn Chart Mesh by Equally Deviding whole Spline
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	bool EnableGenerateMeshAtSplinePoint;

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
	
	//Get Precessed Json Data Container Class
	UFUNCTION(BlueprintCallable, Category = "Chart")
	const UDataClasses* GetDataClassInstance() const
	{
		if (IsDataClassInstanceSet)
			return DataClassInstance;
		else
			return nullptr;
	}
	
	// Set Processed Json Data Container Class Instance Directly : You have to get a reference from other Data3DActor Instance to use this function.
	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetDataClassInstance(UDataClasses* DataClassInstancePtr)
	{
		DataClassInstance = DataClassInstancePtr;
		IsDataClassInstanceSet = true;
	}
	
	// Delete Data Container Class Instance
	UFUNCTION(BlueprintCallable, Category = "Chart")
	void DeleteClassInstance()
	{
		DataClassInstance = nullptr;
		IsDataClassInstanceSet = false;
	}
};