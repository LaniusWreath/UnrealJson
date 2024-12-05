// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JCMDataTypes.h"
#include "JCM3DChartActor.generated.h"

// json을 받아와 DataClass라는 상위 데이터 컨테이너 클래스에 공통으로 저장하고있음.
// 또한 이 공통 데이터는 공통 액터 추상클래스에 속해있음
// 블루프린트에서는 액터가 자식 클래스로 구체화되어있으므로, 데이터 컨테이너 클래스를 호출 할 때에도 액터 형식에 맞게
// 캐스팅해주는 과정이 필요함. 다른 자식 3DActor 클래스에도 구성해줄 것.
// 
// 1205 수정 : 외부로 나가는 get과 set 함수에서는 DataContainer로, 내부에서는 클래스에 맞게 갖고 있게끔 수정

class UJCMDataManager;
class UJCMDataContainer;
class UTextRenderComponent;


UCLASS()
class TEST_API AJCM3DChartActor : public AActor
{
	GENERATED_BODY()

public:
	AJCM3DChartActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const UJCMDataContainer* CallInstancingDataContainer(const TSharedPtr<FJsonObject> JsonData);
	void SetJsonString(const bool IsWorkDone);
	void SetbDataContainerSet(const bool InState);
	const bool GetbDataContainerSet() const;

public:
	// Call json request function, Result data will be stored in actor as container
	UFUNCTION(BlueprintCallable, Category = "Chart")
	void RequestJsonObject(const FString& URL);

	// Call json request function, Result data will be returned as string
	UFUNCTION(BlueprintCallable, Category = "Chart")
	void RequestJsonString(const FString& URL);

	// Load local json file, Result data will be stored in actor as container
	UFUNCTION(BlueprintCallable, Category = "Chart")
	UJCMDataContainer* LoadFromLocalJsonFile(const FString& FilePath);

	// Get Http request handler reference from JCM actor
	UFUNCTION(BlueprintCallable, Category = "Chart")
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponent_chartTitle;

protected:
	// Check JCM actor state
	UFUNCTION(BlueprintCallable, Category = "JCM")
	virtual bool CheckJCMActorIntegrity();

	// Initializing Request Manager Instance : this instance have to be initialized in every CallJsonRoutine
	UFUNCTION(BlueprintCallable, Category = "Chart")
	const UJCMHttpHandler* InitializeRequestHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Initializing Data Manager Getting from Game Instance
	void SetJCMDataManagerRef();

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
};


class UJCMChartGeneratorBar;
class AJCMBarBaseActor;

UCLASS(Blueprintable)
class AJCM3DChartActorBar : public AJCM3DChartActor
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void SetChartDefaultTexts();

	// Data Class Instance ref
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "JCM", meta = (AllowPrivateAccess = "true"))
	UJCMDataContainerBar* DataContainerInstance;

protected:
	// chart genrating function sequence
	UFUNCTION(BlueprintCallable, Category = "Chart")
	virtual void GenerateChartRoutine() override;

public:
	bool CheckJCMActorIntegrity() override;

public:
	AJCM3DChartActorBar();

	// On : Spawn Chart Mesh At Each Spline Point / Off : Spawn Chart Mesh by Equally Deviding whole Spline
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	bool EnableGenerateMeshAtSplinePoint;

	// Select JCM BarBaseActor Blueprint Actor Source to Generate
	UPROPERTY(EditAnywhere, BlueprintReadOnly ,Category = "Chart")
	TSubclassOf<AJCMBarBaseActor> BarBaseActorBPClass;

	// Component for generating JCM Bar Chart 
	UPROPERTY(BlueprintReadOnly, Category = "Chart")
	UJCMChartGeneratorBar* BarGeneratorComponent;

	// Visualization Chart Xaxis Name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponent_chartXaxisName;

	// Visualization Chart Yaxis Name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponent_chartYaxisName;
	
	//Get data container Ref
	UFUNCTION(BlueprintCallable, Category = "Chart")
	const UJCMDataContainerBar* GetDataContainerRef();
	
	// Set Processed Json Data Container Instance Directly
	UFUNCTION(BlueprintCallable, Category = "Chart")
	void SetDataContainer(UJCMDataContainer* DataContainerRef);
	
	// Delete Data Container Instance
	UFUNCTION(BlueprintCallable, Category = "Chart")
	void DeleteDataContainerInstance()
	{
		DataContainerInstance = nullptr;
		bDataContainerSet = false;
	}
};