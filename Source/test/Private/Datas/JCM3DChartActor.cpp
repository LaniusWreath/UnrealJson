	// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/JCM3DChartActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "Datas/JCMCore.h"
#include "Datas/JCMBarBaseActor.h"
#include "Datas/JCMDataManager.h"
#include "Datas/JCMChartGenerator.h"
#include "Datas/JCMDataContainer.h"
#include "Datas/JCMHttpHandler.h"
#include "Components/SplineComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"

/// <summary>
/// BaseActor Functions
/// </summary>

AJCM3DChartActor::AJCM3DChartActor()
{
	UE_LOG(LogTemp, Log, TEXT("Data3DActor : Initializing %s"), *GetName());

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponet"));
	TextRenderComponent_chartTitle = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text_title"));
	TextRenderComponent_chartTitle->SetupAttachment(RootSceneComponent);
	TextRenderComponent_chartTitle->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRenderComponent_chartTitle->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
}

// Called when the game starts or when spawned
void AJCM3DChartActor::BeginPlay()
{
	Super::BeginPlay();
}

// JsonObjectPtr를 받는 함수
void AJCM3DChartActor::RequestJsonObject(const FString& URL)
{
	SetJCMDataManagerRef();
	InitializeRequestManager();
	// Request 델리게이트 바인딩 함수 : DataClass 멤버변수 초기화 
	// 멀티캐스트 델리게이트였다면 델리게이트 객체 매 번 청소해야 겠지만, 지금은 싱글캐스트. 객체 매 번 삭제 안해도 됨.
	RequestManagerInstance->OnParsedJsonObjectPtrReady.BindUObject(this, &AJCM3DChartActor::SetJsonObject);
	IsDataClassInstanceSet = false;
	RequestManagerInstance->MakeGetRequest(URL, false);
}

// FString을 받는 함수
void AJCM3DChartActor::RequestJsonString(const FString& URL)
{
	SetJCMDataManagerRef();
	InitializeRequestManager();
	RequestManagerInstance->OnRequestedJsonStringReady.BindUObject(this, &AJCM3DChartActor::SetJsonString);
	IsDataClassInstanceSet = false;
	RequestManagerInstance->MakeGetRequest(URL);
}

// 로컬 Json 읽어 데이터 컨테이너 세팅
void AJCM3DChartActor::LoadFromLocalJsonFile(const FString& FilePath)
{
	SetJCMDataManagerRef();
	IsDataClassInstanceSet = false;
	DataClassInstance = DataManagerInstanceRef->InstancingDataContainerFromLocalJson(FilePath);
}

// 기본 액터 무결성 체크 함수
bool AJCM3DChartActor::CheckJCMActorIntegrity()
{
	bool bIsValid= true;
	if (!DataManagerInstanceRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Integrity Check : %s : DataManager Invalid "), *this->GetName());
		bIsValid = false;
	}
	if (!DataClassInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Integrity Check : %s : DataContainer Invalid"), *this->GetName());
		bIsValid = false;
	}
	if (!RequestManagerInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Integrity Check : %s : HttpHandler Instance Invalid"), *this->GetName());
		bIsValid = false;
	}

	return bIsValid;
}

void AJCM3DChartActor::SetJCMDataManagerRef()
{	
	if (!DataManagerInstanceRef)
	{
		if (UJCMCore::GetJCMCore())
		{
			DataManagerInstanceRef = UJCMCore::GetJCMCore()->GetDataManager();
			if (!DataManagerInstanceRef)
			{
				UE_LOG(LogTemp, Warning, TEXT("Data3DActor : Initialize Managers : Getting DataManager Reference Failed"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Couldn't find JCMCore"));
		}
	}
}

// RequestManager의 인스턴스는 각각 request를 담당하는 액터에서 직접 생성. 
// 각 인스턴스의 델리게이트는 각 액터의 url에 대한 리퀘스트만 감지하여 각 액터 인스턴스의 함수와 1:1 바인딩
// url로 데이터를 리퀘스트 할 때 마다 기존 httpManager인스턴스를 삭제하고 새롭게 초기화.
void AJCM3DChartActor::InitializeRequestManager()
{
	if (!RequestManagerInstance)
	{
		RequestManagerInstance = NewObject<UJCMHttpHandler>();
	}
}

// FJsonObject 받아 UDataClasses*로 추출
void AJCM3DChartActor::SetJsonObject(const TSharedPtr<FJsonObject> JsonData)
{
	if (RequestManagerInstance)
	{
		//TSharedPtr<FJsonObject> Data = RequestManagerInstance->GetJsonData();
		FDataInstancePair ResultData = DataManagerInstanceRef->InstancingDataClass(JsonData);
		UE_LOG(LogTemp, Log, TEXT("Data3DChartActor : DataClass Set, Response Chart Class is : %s"), *ResultData.ClassName);
		DataClassInstance = ResultData.DataInstance;
		if (!DataClassInstance)
		{
			UE_LOG(LogTemp, Log, TEXT("Data3DChartActor : Received Data Class is invaid"));
		}
		else
		{
			IsDataClassInstanceSet = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Data3DChartActor : RequestManagerInstance is Invalid"));
	}
}

void AJCM3DChartActor::SetJsonString(const bool IsWorkDone)
{
	IsRequestJsonStringDone = IsWorkDone;
}


// Called every frame
void AJCM3DChartActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AJCM3DChartActorBar::AJCM3DChartActorBar()
{
	EnableGenerateMeshAtSplinePoint = false;
	BarGeneratorComponent = CreateDefaultSubobject<UJCMChartGeneratorBar>(TEXT("BarGeneratorComponent"));
	// 처음에는 ChartGenerator 생성자에서 각각 컴포넌트 붙였으나, 계층 구조만 변경될 뿐 실제로 붙지 않는 문제 발생.
	BarGeneratorComponent->SetupAttachment(RootSceneComponent);
	// ChartGenerator의 멤버 컴포넌트 따로 부착해주는 함수 작성
	BarGeneratorComponent->SetAttachComponents(BarGeneratorComponent);

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
void AJCM3DChartActorBar::SetChartDefaultTexts()
{
	if (DataClassInstance)
	{
		UJCMDataContainerBar* TempCastedDataClass = Cast<UJCMDataContainerBar>(DataClassInstance);
		//FString ChartTitle = TempCastedDataClass->GetChartTitle();
		FString ChartTitle = TempCastedDataClass->GetShapeChartData().ChartTitle;

		TextRenderComponent_chartTitle->SetText(FText::FromString(ChartTitle));
		UE_LOG(LogTemp, Log, TEXT("Data3DActor : Chart Title : %s"), *ChartTitle);

		FString ChartXAxisName = TempCastedDataClass->GetShapeChartData().XName;
		TextRenderComponent_chartXaxisName->SetText(FText::FromString(ChartXAxisName));

		FString ChartYAxisName = TempCastedDataClass->GetShapeChartData().YName;
		TextRenderComponent_chartYaxisName->SetText(FText::FromString(ChartYAxisName));
	}
}

void AJCM3DChartActorBar::GenerateChartRoutine()
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
	if (BarGeneratorComponent && BarBaseActorBPClass)
	{
		BarGeneratorComponent->SetBarSourceActor(BarBaseActorBPClass);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Data3DActor : BarBaseActorBPClass or BarGeneratorComponent is null"));
		return;
	}

	// 데이터로부터 차트 타입 추출
	EJCMChartTypes ECurrentChartType = DataClassInstance->GetChartType();
	UE_LOG(LogTemp, Log, TEXT("Data3DActorBar : LastChartType is %d"), ECurrentChartType);

	// 바 데이터 객체로 데이터 클래스 객체 캐스팅
	UJCMDataContainerBar* BarDataClassInstance = Cast<UJCMDataContainerBar>(DataClassInstance);

	// GenerateBarChart() : 데이터 입력 받아 차트 생성 루틴 함수 호출 / GetShapeChartData() : Bar(모양)차트 데이터 Get
	BarGeneratorComponent->GenerateBarChart(BarDataClassInstance->GetShapeChartData(), EnableGenerateMeshAtSplinePoint);

}

// Bar 액터 무결성 체크 함수
bool AJCM3DChartActorBar::CheckJCMActorIntegrity()
{
	bool bIsValid = Super::CheckJCMActorIntegrity();

	if (!BarGeneratorComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Integrity Check : %s : BarGeneratorComponent Invalid"), *this->GetName());
		bIsValid = false;
	}
	if (!BarBaseActorBPClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Integrity Check : %s : BarBaseActorBPClass Invalid"), *this->GetName());
		bIsValid = false;
	}

	return bIsValid;
}