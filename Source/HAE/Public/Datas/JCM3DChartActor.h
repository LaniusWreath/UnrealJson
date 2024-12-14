// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JCMDataTypes.h"
#include "JCM3DChartActor.generated.h"


class UJCMDataManager;
class UJCMDataContainer;
class UTextRenderComponent;

UCLASS()
class HAE_API AJCM3DChartActor : public AActor
{
	GENERATED_BODY()

public:
	AJCM3DChartActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetJsonString(const bool IsWorkDone);
	void SetbDataContainerSet(const bool InState);
	const bool GetbDataContainerSet() const;

public:
	// Call json request function, Result data will be stored in actor as container
	UFUNCTION(BlueprintCallable, Category = "JCM")
	void RequestJsonObject(const FString& URL);

	// Call json request function, Result data will be returned as string
	UFUNCTION(BlueprintCallable, Category = "JCM")
	void RequestJsonString(const FString& URL);

	// Load local json file, Result data will be stored in actor as container
	UFUNCTION(BlueprintCallable, Category = "JCM")
	UJCMDataContainer* LoadFromLocalJsonFile(const FString& FilePath);

	// Get Http request handler reference from JCM actor
	UFUNCTION(BlueprintCallable, Category = "JCM")
	const UJCMHttpHandler* GetHttpRequestHandler() const
	{
		if (RequestHandlerInstance) {
			return RequestHandlerInstance;
		}
		else
			return nullptr;
	}
	
public:
	// Visualizating Chart Title
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "JCM")
	UTextRenderComponent* TextRenderComponent_chartTitle;

protected:
	// Check JCM actor state
	UFUNCTION(BlueprintCallable, Category = "JCM")
	virtual bool CheckJCMActorIntegrity();

	// Initializing Request Manager Instance : this instance have to be initialized in every CallJsonRoutine
	UFUNCTION(BlueprintCallable, Category = "JCM")
	const UJCMHttpHandler* InitializeRequestHandler();

	UFUNCTION(BlueprintCallable, Category = "JCM")
	virtual void UpdateChartRoutine() { return; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Initializing Data Manager Getting from Game Instance
	void SetJCMDataManagerRef();

	// Http request Delegate binding function
	virtual void CallInstancingDataContainer(const TSharedPtr<FJsonObject> JsonData);

	// Pure Virtual Routine for Generate Chart
	virtual void GenerateChartRoutine() PURE_VIRTUAL(UDataFetcherBase::FetchData, ;);

protected:
	// JCM DataManager reference
	UPROPERTY()
	UJCMDataManager* DataManagerInstanceRef;

	// HttpRequest Handler Instance ref
	UPROPERTY()
	UJCMHttpHandler* RequestHandlerInstance;

	UPROPERTY()
	USceneComponent* RootSceneComponent;

	bool IsRequestJsonStringDone;

	// Data Container State
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "JCM")
	bool bDataContainerSet;

private:
	UPROPERTY(BlueprintReadOnly, Category = "JCM", meta = (AllowPrivateAccess = "true"))
	UJCMDataContainer* DataContainer;

};


class UJCMChartGeneratorBar;
class AJCMBarBaseActor;

UCLASS(Blueprintable)
class AJCM3DChartActorBar : public AJCM3DChartActor
{
	GENERATED_BODY()

private:
	void SetChartDefaultTexts();

private:
	// Data Class Instance ref
	UPROPERTY(BlueprintReadOnly, Category = "JCM", meta = (AllowPrivateAccess = "true"))
	UJCMDataContainerBar* DataContainerBar;

protected:
	// chart genrating function sequence
	UFUNCTION(BlueprintCallable, Category = "JCM")
	virtual void GenerateChartRoutine() override;

public:
	AJCM3DChartActorBar();

	bool CheckJCMActorIntegrity() override;

	// Http request Delegate binding function
	void CallInstancingDataContainer(const TSharedPtr<FJsonObject> JsonData) override;

public:
	// On : Spawn Chart Mesh At Each Spline Point / Off : Spawn Chart Mesh by Equally Deviding whole Spline
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JCM")
	bool EnableGenerateMeshAtSplinePoint;

	// Select JCM BarBaseActor Blueprint Actor Source to Generate
	UPROPERTY(EditAnywhere, BlueprintReadOnly ,Category = "JCM")
	TSubclassOf<AJCMBarBaseActor> BarBaseActorBPClass;

	// Component for generating JCM Bar Chart 
	UPROPERTY(BlueprintReadOnly, Category = "JCM")
	UJCMChartGeneratorBar* ChartGeneratorComponent;

	// Visualization Chart Xaxis Name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "JCM")
	UTextRenderComponent* TextRenderComponent_chartXaxisName;

	// Visualization Chart Yaxis Name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "JCM")
	UTextRenderComponent* TextRenderComponent_chartYaxisName;
	
public:
	//Get data container Ref
	UFUNCTION(BlueprintCallable, Category = "JCM")
	const UJCMDataContainerBar* GetDataContainerRef();
	
	// Set Processed Json Data Container Instance Directly
	UFUNCTION(BlueprintCallable, Category = "JCM")
	void SetDataContainer(UJCMDataContainer* DataContainerRef);
	
	// Delete Data Container Instance
	UFUNCTION(BlueprintCallable, Category = "JCM")
	void DeleteDataContainerInstance()
	{
		ChartGeneratorComponent = nullptr;
		bDataContainerSet = false;
	}

	void UpdateChartRoutine() override;
};