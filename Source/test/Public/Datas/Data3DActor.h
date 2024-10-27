// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTypes.h"
#include "Data3DActor.generated.h"

class UDataManager;
class UDataClasses;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chart")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chart")
	FString CurrentChartType;

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void CallJsonObject(const FString& URL);

	// Initializing Data Manager Getting from Game Instance
	UFUNCTION()
	void InitilizeManagers();

	void SetJsonObject (const TSharedPtr<FJsonObject> JsonData);

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

	// Controler Component for Generating 3D Bar Chart 
	UPROPERTY()
	UBarGenerator* BarGeneratorComponent;

protected:
	//virtual UDataClasses* SetDataClassInstance() override;
	UFUNCTION(BlueprintCallable, Category = "Chart")
	virtual void GenerateChartRoutine() override;

public:
	AData3DActorBar();

	// Select Bar Blueprint Actor Source to Generate
	UPROPERTY(EditAnywhere, BlueprintReadOnly ,Category = "Chart")
	TSubclassOf<ABarBaseActor> BarBaseActorBPClass;

	// Visualization Chart Xaxis Name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponent_chartXaxisName;

	// Visualization Chart Yaxis Name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponent_chartYaxisName;

};