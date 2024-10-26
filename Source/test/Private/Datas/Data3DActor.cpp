	// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/Data3DActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "Datas/JBCMCore.h"
#include "Datas/BarBaseActor.h"
#include "Datas/DataManager.h"
#include "Datas/ChartGenerator.h"
#include "Datas/DataClasses.h"
#include "Datas/HTTPRequestManager.h"
#include "Components/SplineComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"

/// <summary>
/// BaseActor Functions
/// </summary>

AData3DActor::AData3DActor()
{
	UE_LOG(LogTemp, Log, TEXT("Data3DActor : Initializing %s"), *GetName());

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponet"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootSceneComponent);

	TextRenderComponent_chartTitle = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text_title"));
	TextRenderComponent_chartTitle->SetupAttachment(RootSceneComponent);
	TextRenderComponent_chartTitle->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRenderComponent_chartTitle->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
}

// Called when the game starts or when spawned
void AData3DActor::BeginPlay()
{
	Super::BeginPlay();

	InitilizeManagers();
}

void AData3DActor::CallJsonObjectPtr(const FString& URL)
{
	RequestManagerInstance->MakeGetRequest(URL);

}

void AData3DActor::InitilizeManagers()
{	
	UJBCMCore* JBCMCoreReference = UJBCMCore::GetJBCMCore();

	if (JBCMCoreReference)
	{
		DataManagerInstance = JBCMCoreReference->GetDataManager();
		if (!DataManagerInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Data3DActor : Initialize Managers : Getting DataManager Reference Failed"));
		}
		RequestManagerInstance = JBCMCoreReference->GetHttpRequestManager();
		if (!RequestManagerInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Data3DActor : Initialize Managers : Getting RequestManager Reference Failed"));
		}
		else
		{
			// Request 델리게이트 바인딩 함수 : DataClass 멤버변수 초기화
			RequestManagerInstance->OnParsedDataReady.BindUObject(this, &AData3DActor::SetJsonObject);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find GameInstance or DataManager"));
	}
}

// FJsonObject 받아 UDataClasses*로 추출
void AData3DActor::SetJsonObject(const TSharedPtr<FJsonObject> JsonData)
{
	if (RequestManagerInstance)
	{
		TSharedPtr<FJsonObject> Data = RequestManagerInstance->GetJsonData();
		FDataInstancePair ResultData = DataManagerInstance->InstancingDataClass(Data);
		UE_LOG(LogTemp, Log, TEXT("Data3DActor : Response Chart Class is : %s"), *ResultData.ClassName);
		DataClassInstance = ResultData.DataInstance;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Data3DActor : RequestManagerInstance is Invalid"));
	}
}

// Called every frame
void AData3DActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AData3DActorBar::AData3DActorBar()
{
	BarGeneratorComponent = CreateDefaultSubobject<UBarGenerator>(TEXT("barGeneratorComponent"));
	if (RootSceneComponent)
	{
		// 처음에는 ChartGenerator 생성자에서 각각 컴포넌트 붙였으나, 계층 구조만 변경될 뿐 실제로 붙지 않는 문제 발생.
		// -> 액터에서 직접 컴포넌트의 구성 컴포넌트들 액터의 RootComponent에 Attach
		BarGeneratorComponent->SetupAttachment(RootSceneComponent);
		BarGeneratorComponent->SplineComponent_height->SetupAttachment(BarGeneratorComponent);
		BarGeneratorComponent->SplineComponent_length->SetupAttachment(BarGeneratorComponent);
		BarGeneratorComponent->ChildActorContainComponent->SetupAttachment(BarGeneratorComponent);
	}
	TextRenderComponent_chartXaxisName = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text_xAxis"));
	TextRenderComponent_chartXaxisName->SetupAttachment(RootSceneComponent);
	TextRenderComponent_chartXaxisName->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRenderComponent_chartXaxisName->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);

	TextRenderComponent_chartYaxisName = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text_yAxis"));
	TextRenderComponent_chartYaxisName->SetupAttachment(RootSceneComponent);
	TextRenderComponent_chartYaxisName->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRenderComponent_chartYaxisName->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
}

// 차트 타이틀, X축, Y축 이름 초기화 함수
void AData3DActorBar::SetChartDefaultTexts()
{
	if (DataClassInstance)
	{
		UShapeChartClass* TempCastedDataClass = Cast<UShapeChartClass>(DataClassInstance);
		FString ChartTitle = TempCastedDataClass->GetChartTitle();
		TextRenderComponent_chartTitle->SetText(FText::FromString(ChartTitle));
		UE_LOG(LogTemp, Log, TEXT("Data3DActor : Chart Title : %s"), *ChartTitle);

		FString ChartXAxisName = TempCastedDataClass->GetChartXName();
		TextRenderComponent_chartXaxisName->SetText(FText::FromString(ChartXAxisName));

		FString ChartYAxisName = TempCastedDataClass->GetChartYName();
		TextRenderComponent_chartYaxisName->SetText(FText::FromString(ChartYAxisName));
	}
}

//UDataClasses* AData3DActorBar::SetDataClassInstance()
//{
//	if (DataManagerInstance)
//	{
//		UE_LOG(LogTemp, Log, TEXT("Data3DActor : Getting Last Chart Data Class Instance"));
//		DataClassInstance = DataManagerInstance->GetChartDataClassInstance(ChartClassNames::NAME_BARCHART);
//		if (DataClassInstance)
//		{
//			UE_LOG(LogTemp, Log, TEXT("Data3DActor : DataClassInstance : %s"), *DataClassInstance->GetName());
//			return DataClassInstance;
//		}
//		else
//		{
//			UE_LOG(LogTemp, Warning, TEXT("Data3DActor : DataClassInstacning Failed"));
//			return nullptr;
//		}
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("Data3DActor : no DataManagerReference"));
//		return nullptr;
//	}
//}

void AData3DActorBar::GenerateChartRoutine()
{

	UE_LOG(LogTemp, Log, TEXT("Data3DActorBar : Generate Chart Routine Running"));
	if (!DataClassInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Data3DActorBar : GenerateChartRoutine : DataClassInstance is invalid"));
		return;
	}

	// 차트 타이틀 초기화
	SetChartDefaultTexts();

	// 생성할 바 소스 액터 BarGenerator에 전달
	if (BarBaseActorBPClass)
	{
		BarGeneratorComponent->SetBarSourceActor(BarBaseActorBPClass);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Data3DActor : BarBaseActorBPClass is null"));
		return;
	}

	// 데이터로부터 차트 타입 추출
	EChartTypes ECurrentChartType = DataClassInstance->GetChartType();
	UE_LOG(LogTemp, Log, TEXT("Data3DActorBar : LastChartType is %d"), ECurrentChartType);

	// 바 데이터 객체로 데이터 클래스 객체 캐스팅
	UShapeChartBarClass* BarDataClassInstance = Cast<UShapeChartBarClass>(DataClassInstance);

	// GenerateBarChart() : 데이터 입력 받아 차트 생성 루틴 함수 호출 / GetShapeChartData() : Bar(모양)차트 데이터 Get
	BarGeneratorComponent->GenerateBarChart(BarDataClassInstance->GetShapeChartData());

}


