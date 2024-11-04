// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/ChartGenerator.h"
#include "Components/SplineComponent.h"
#include "Datas/BarBaseActor.h"
#include "Algo/MaxElement.h"
#include "Components/TextRenderComponent.h"


// 기본 차트 베이스 초기화
UChartGenerator::UChartGenerator()
{
	ChildActorContainComponent= CreateDefaultSubobject<USceneComponent>(TEXT("childActorContainComponent"));
	ChildActorContainComponent->SetupAttachment(GetAttachParent());
	UE_LOG(LogTemp, Log, TEXT("%s"), *(GetAttachmentRoot()->GetName()));
	ChildActorContainComponent->SetMobility(EComponentMobility::Movable);
}

void UChartGenerator::BeginPlay()
{

}

// Base에 붙은 액터 삭제
void UChartGenerator::ClearChildrenActors()
{
	int32 ExistActors = ChildActorComponents.Num();

	for (UChildActorComponent* ChildComponent : ChildActorComponents)
	{
		if (ChildComponent && ChildComponent->GetChildActor())
		{
			UE_LOG(LogTemp, Log, TEXT("ChartGenerator : Children Actor %s cleard"), *ChildComponent->GetChildActor()->GetName());
			ChildComponent->GetChildActor()->Destroy();
			ChildComponent->DestroyComponent();
		}
	}
	UE_LOG(LogTemp, Log, TEXT("All Children Actors cleard"));

	ChildActorComponents.Empty();
}

////////////////////////////////////////////////////////////////////////////////////////

UBarGenerator::UBarGenerator()
{
	// 차트 들어가는 각 컴포넌트 인스턴스 생성만, Attach는 Data3DActor에서 할 것
	SplineComponent_length = CreateDefaultSubobject<USplineComponent>(TEXT("LengthSplineComponent"));
	SplineComponent_length->SetMobility(EComponentMobility::Movable);

	SplineComponent_height = CreateDefaultSubobject<USplineComponent>(TEXT("HeightSplineComponent"));
	SplineComponent_height->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	SplineComponent_height->SetMobility(EComponentMobility::Movable);
}

void UBarGenerator::SetBarSourceActor(const TSubclassOf<ABarBaseActor>& SourceActor)
{
	BarBaseActorBPClass = SourceActor;
}

// 바 차트 생성 함수
void UBarGenerator::GenerateBarChart(const FShapeChartData& CopiedData, bool bGenerateMeshAtSplinePoint)
{
	// 스플라인 총 길이 
	float SplineLength = SplineComponent_length->GetSplineLength();
	UE_LOG(LogTemp, Log, TEXT("ChartGenerator : SplineComponent is %f"), SplineLength);

	// 차트 최대 높이
	float MaxHeight = SplineComponent_height->GetSplineLength();
	UE_LOG(LogTemp, Log, TEXT("ChartGenerator : SplineHeight is %f"), MaxHeight);

	TArray<float> ValueArray = CopiedData.Values;
	TArray<FString> LabelAarray = CopiedData.Labels;

	// 스플라인 사이 간격
	float SplineSpacing = SplineLength / (ValueArray.Num() - 1);

	// 바 타입 차트 생성 사전 준비
	float BarHeightScaler = 0;
	float BarPadding = 0;
	PrepareBarValues(ValueArray, BarHeightScaler, BarPadding, MaxHeight);
	UE_LOG(LogTemp, Log, TEXT("ChartGenerator: PrepareBarValues() result, BarHeightScaler : %f"), BarHeightScaler);

	bool isGenerateDone;
	if (!bGenerateMeshAtSplinePoint)
	{	
		// 스플라인 균등하게 분할하여 메쉬 생성
		isGenerateDone = CreateBar(ValueArray, LabelAarray, SplineSpacing, BarPadding, BarHeightScaler);
	}
	else
	{
		// 스플라인 포인트에 메쉬 생성
		isGenerateDone = CreateBarAlongSplinePoint(ValueArray, LabelAarray, BarPadding, BarHeightScaler);
	}
	// 바 메시 생성
	if (!isGenerateDone)
	{
		UE_LOG(LogTemp, Log, TEXT("ChartGenerator : Generating Bar Failed"));
		return;
	}
}

// 바 차트 생성 전 전처리 함수
void UBarGenerator::PrepareBarValues(const TArray<float>& ValueArray, float& BarHeightScalerResult, float& BarPaddingResult,
	const float MaxHeight)
{
	UE_LOG(LogTemp, Log, TEXT("ChartGenerator : Preperating Bar Chart"));

	int32 Numbers = ValueArray.Num();

	float Range = *Algo::MaxElement(ValueArray) - *Algo::MinElement(ValueArray);
	//UE_LOG(LogTemp, Log, TEXT("ChartGenerator : Range is : %f - %f = %f"), , *Algo::MinElement(ValueArray), Range);
	// 100 * 0.9 / 500
	BarHeightScalerResult = MaxHeight * CustomScaleValue / Range;
	UE_LOG(LogTemp, Log, TEXT("ChartGenerator : Preparing Bar Height Scaler : %f * %f / %f"), MaxHeight, CustomScaleValue, Range);

	BarPaddingResult = MaxHeight * CustomPaddingScaleValue;
	// 로그 스케일링, 정규화 따로 파라미터 빼서 고를 수 있게 할 것
}

bool UBarGenerator::CreateBar(const TArray<float>& ValueArray, const TArray<FString>& LabelArray, const int BarSpacing, 
	const float BarPaddingScaler, const float BarHeightScaler)
{
	ClearChildrenActors();
	int32 Numbers = ValueArray.Num();

	// 바 차트 생성 : ValueArray, AverageHeight, BarHeightScaler
	for (int32 i = 0; i < Numbers; i++)
	{
		float CurrentValue = ValueArray[i];
		float ScaledHeight = CurrentValue * BarHeightScaler;
		UE_LOG(LogTemp, Log, TEXT("ChartGenerator : CurrentValue : %f, BarHeightScaler : %f, ScaledHeight : %f"), 
			CurrentValue, BarHeightScaler ,ScaledHeight);
		float Distance = i * BarSpacing;

		// 스플라인 로컬 좌표를 따야 레벨에 배치했을 때 차트 메시의 좌표에 월드 오프셋이 추가 안됨.
		FVector BarLocation = SplineComponent_length->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
		FString LabelName = LabelArray[i];

		// 자손 액터(차트 액터) 넣을 UChildActorComponent* 반복 생성
		UChildActorComponent* NewChildActorComponent = NewObject<UChildActorComponent>(this);

		if (NewChildActorComponent)
		{
			// 자손 컴포넌트 부착
			//NewChildActorComponent->SetupAttachment(ChildActorContainComponent);
			NewChildActorComponent->AttachToComponent(ChildActorContainComponent, FAttachmentTransformRules::KeepRelativeTransform);

			//자손 액터 클래스 설정
			NewChildActorComponent->SetChildActorClass(BarBaseActorBPClass);

			// 자손 액터 생성
			NewChildActorComponent->CreateChildActor();

			UE_LOG(LogTemp, Log, TEXT("ChartGenerator : Creating Bar Child Object : %s"), 
				*NewChildActorComponent->GetChildActorClass()->GetName());

			// 배열에 추가
			ChildActorComponents.Add(NewChildActorComponent);

			if (NewChildActorComponent->GetChildActor())
			{
				// ABarBaseAcotr로 UChildActorComponent 캐스팅
				ABarBaseActor* ChildBar = Cast<ABarBaseActor>(NewChildActorComponent->GetChildActor());
				if (ChildBar)
				{
					// 이동 : 이동 먼저 시켜줘야 생성 좌표가 고정됨
					ChildBar->SetActorRelativeLocation(BarLocation);
					// 바 프로시저럴 메쉬 생성
					ChildBar->CreateMesh(ScaledHeight, CurrentValue);
					// 바 라벨 텍스트 렌더러 값 초기화
					ChildBar->InitializeTextMeshLabel(LabelName); 
					// 바 값 텍스트 렌더러 값 초기화
					ChildBar->InitializeTextMeshValue(CurrentValue);
					// 애니메이션
					ChildBar->PlayBarAnimation();
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("ChartGenerator: Failed Casting ChildBarBaseActor"));
					return false;
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ChartGenerator : Failed NewChildActorComponent->GetChildActor()"));
			}
		}

		UE_LOG(LogTemp, Log, TEXT("ChartGenerator: Created bar Number with Height: %f"), ScaledHeight);
	}
	return true;
}

// 스플라인 컴포넌트의 점 개수 모자랄 때 target만큼 맞춰줌
void UBarGenerator::AddSplinePoint(USplineComponent* SplineComponent, int TargetCount)
{
	int32 SplinePointCount = SplineComponent->GetNumberOfSplinePoints();
	if (TargetCount > SplinePointCount)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ChartGenerator : CreateBarAlongSplinePoint Value Count %d dosen't same as SplinePoint Count %d"),
			TargetCount, SplinePointCount);
		UE_LOG(LogTemp, Warning,
			TEXT("Adding Extra Spline Point Automatically"));

		FVector LastSplinePointVector = SplineComponent_length->
			GetLocationAtSplinePoint(SplinePointCount - 1, ESplineCoordinateSpace::Local);
		UE_LOG(LogTemp, Log, TEXT("LastSplinePointVector : %f, %f, %f"),
			LastSplinePointVector.X, LastSplinePointVector.Y, LastSplinePointVector.Z);

		FVector UnitOffsetVector =
			LastSplinePointVector - SplineComponent_length->
			GetLocationAtSplinePoint(SplinePointCount - 2, ESplineCoordinateSpace::Local);
		UE_LOG(LogTemp, Log, TEXT("UnitOffsetVector : %f, %f, %f"), UnitOffsetVector.X, UnitOffsetVector.Y, UnitOffsetVector.Z);

		for (int i = 0; i < TargetCount - SplinePointCount; i++)
		{
			SplineComponent_length->AddSplinePoint(LastSplinePointVector + (UnitOffsetVector * (i + 1)),
				ESplineCoordinateSpace::Local);
		}
	}
}


bool UBarGenerator::CreateBarAlongSplinePoint(const TArray<float>& ValueArray, const TArray<FString>& LabelArray, 
	const float BarPaddingScaler, const float BarHeightScaler)
{
	// 기존 bar 객체 삭제
	ClearChildrenActors();

	int32 Numbers = ValueArray.Num();
	int32 SplinePointCount = SplineComponent_length->GetNumberOfSplinePoints();		// 스플라인 포인트 개수

	// 데이터 수에 스플라인 포인트 개수 맞춤
	AddSplinePoint(SplineComponent_length, Numbers);

	// 바 차트 생성 : ValueArray, AverageHeight, BarHeightScaler
	for (int32 i = 0; i < Numbers; i++)
	{
		float CurrentValue = ValueArray[i];
		float ScaledHeight = CurrentValue * BarHeightScaler;
		UE_LOG(LogTemp, Log, TEXT("ChartGenerator : CurrentValue : %f, BarHeightScaler : %f, ScaledHeight : %f"),
			CurrentValue, BarHeightScaler, ScaledHeight);

		// 스플라인 로컬 좌표를 따야 레벨에 배치했을 때 차트 메시의 좌표에 월드 오프셋이 추가 안됨.
		FVector BarLocation = SplineComponent_length->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FString LabelName = LabelArray[i];

		// 자손 액터(차트 액터) 넣을 UChildActorComponent* 반복 생성
		UChildActorComponent* NewChildActorComponent = NewObject<UChildActorComponent>(this);

		if (NewChildActorComponent)
		{
			// 자손 컴포넌트 부착
			//NewChildActorComponent->SetupAttachment(ChildActorContainComponent);
			NewChildActorComponent->AttachToComponent(ChildActorContainComponent, FAttachmentTransformRules::KeepRelativeTransform);

			//자손 액터 클래스 설정
			NewChildActorComponent->SetChildActorClass(BarBaseActorBPClass);

			// 자손 액터 생성
			NewChildActorComponent->CreateChildActor();

			UE_LOG(LogTemp, Log, TEXT("ChartGenerator : Creating Bar Child Object : %s"),
				*NewChildActorComponent->GetChildActorClass()->GetName());

			// 배열에 추가
			ChildActorComponents.Add(NewChildActorComponent);

			if (NewChildActorComponent->GetChildActor())
			{
				// ABarBaseAcotr로 UChildActorComponent 캐스팅
				ABarBaseActor* ChildBar = Cast<ABarBaseActor>(NewChildActorComponent->GetChildActor());
				if (ChildBar)
				{
					// 이동 : 이동 먼저 시켜줘야 생성 좌표가 고정됨
					ChildBar->SetActorRelativeLocation(BarLocation);
					// 바 프로시저럴 메쉬 생성
					ChildBar->CreateMesh(ScaledHeight, CurrentValue);
					// 바 라벨 텍스트 렌더러 초기화
					ChildBar->InitializeTextMeshLabel(LabelName);
					// 바 값 텍스트 렌더러 초기화
					ChildBar->InitializeTextMeshValue(CurrentValue);
					// 애니메이션
					ChildBar->PlayBarAnimation();
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("ChartGenerator: Failed Casting ChildBarBaseActor"));
					return false;
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ChartGenerator : Failed NewChildActorComponent->GetChildActor()"));
			}

		}

		UE_LOG(LogTemp, Log, TEXT("ChartGenerator: Created bar Number with Height: %f"), ScaledHeight);
	}

	return false;
}


