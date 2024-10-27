// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/BarBaseActor.h"
#include "ProceduralMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/TextRenderComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "PhysicsEngine/BodySetup.h"

// Sets default values
ABarBaseActor::ABarBaseActor()
{
	DefaultSceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = DefaultSceneRootComponent;

	ProcMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	ProcMeshComponent->SetupAttachment(RootComponent);

	CustomActorSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LegacyMeshOffsetLayer"));
	CustomActorSceneComponent->SetupAttachment(RootComponent);

	CustomStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegacyMesh"));
	CustomStaticMeshComponent->SetupAttachment(CustomActorSceneComponent);

	CustomStaticMeshComponent->SetHiddenInGame(true);

	// Procedural Mesh Component를 Navigation 시스템에서 제외. 경로탐색이나 ai상호작용이 필요 없는 경우, 꺼도 좋음. 
	// 안끄면 Navigation system에서 화냄. (계속 감시중)
	ProcMeshComponent->SetCanEverAffectNavigation(false);
	BarAnimationTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("BarAnimationTimeline"));

	// 텍스트 렌더러 - 값
	TextRenderComponentValue = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponentValue"));
	TextRenderComponentValue->SetupAttachment(RootComponent);

	// 텍스트 피벗을 가운데로 설정
	TextRenderComponentValue->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRenderComponentValue->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	
	// 텍스트 렌더러 - 라벨
	TextRenderComponentLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponentLabel")); 
	TextRenderComponentLabel->SetupAttachment(RootComponent);

	TextRenderComponentLabel->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRenderComponentLabel->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
}

// Called when the game starts or when spawned
void ABarBaseActor::BeginPlay()
{
	Super::BeginPlay();

}

void ABarBaseActor::CreateProceduralBoxMesh(float BarHeight)
{
	// 필요한 배열 선언
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> Tangents;

	float BarWidth = UnitSize*Width_bar;
	float w = BarWidth / 2;

	// 박스의 크기 설정 (박스 너비)
	FVector BoxRadius(w, w, w); // 박스의 절반 크기

	// 박스 메쉬 생성
	UKismetProceduralMeshLibrary::GenerateBoxMesh(BoxRadius, Vertices, Triangles, Normals, UV0, Tangents);

	// 박스 높이만큼 스케일 계산해서 높이 올림.
	float zScaler = BarHeight / (UnitSize*BarWidth);
	UE_LOG(LogTemp, Log, TEXT("ABarBaseActor : Calculate ZScaler : zScaler : %f, BarHeight : %f, UnitSIZE : %d, BarWidth : %f"), zScaler, BarHeight, UnitSize, BarWidth);
	//UE_LOG(LogTemp, Log, TEXT("BarBaseActor : zScaler is %f"), zScaler);
	ProcMeshComponent->SetWorldScale3D(FVector(1.f, 1.f, zScaler));
	ProcMeshComponent->AddWorldOffset(FVector(0, 0, (BarHeight / 2)));

	// Procedural Mesh Component에 메시 생성
	ProcMeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, {}, Tangents, true);

	if (MeshMaterial)
	{
		ProcMeshComponent->SetMaterial(0, MeshMaterial);
	}
}

// 커스텀 메시를 차트 메시로 할 경우, 높이 매핑하여 메시 수정
void ABarBaseActor::CreateCustomMesh(float BarHeight)
{
	float UnitMeshHeight = GetStaticMeshBoxUnitSize(CustomStaticMeshComponent->GetStaticMesh()).Z;
	int32 UnitMeshAmount = BarHeight / UnitMeshHeight;
	UE_LOG(LogTemp, Log, TEXT("BarBaseActor : Amount is %d"), UnitMeshAmount);
	float UnitMeshLeft = BarHeight / UnitMeshHeight - UnitMeshAmount;

	for (int i = 0; i < UnitMeshAmount; i++)
	{
		// StaticMeshComponent를 동적으로 생성하고, 부모 액터에 속하도록 설정
		UStaticMeshComponent* UnitMeshComponent = NewObject<UStaticMeshComponent>(this);

		// 템플릿의 속성을 UnitMeshComponent에 복사
		UnitMeshComponent->SetStaticMesh(CustomStaticMeshComponent->GetStaticMesh());
		UnitMeshComponent->SetMaterial(0, CustomStaticMeshComponent->GetMaterial(0));
		UnitMeshComponent->SetRelativeScale3D(CustomStaticMeshComponent->GetRelativeScale3D());

		// 콜리전
		UnitMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		UnitMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
		UnitMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);

		// 중력
		UnitMeshComponent->SetSimulatePhysics(true);
		UnitMeshComponent->SetEnableGravity(true);

		UnitMeshComponent->SetupAttachment(CustomActorSceneComponent);
		UnitMeshComponent->AddWorldOffset(FVector(0, 0, BarHeight+ UnitMeshHeight*3));
		// 월드에 컴포넌트를 등록하여 액터와 함께 관리되도록 설정
		UnitMeshComponent->RegisterComponent();
	}
}

UStaticMeshComponent* ABarBaseActor::InitializeCustomStaticMeshPhysics(UStaticMeshComponent* TargetStaticMesh)
{
	UBodySetup* BodySetup = TargetStaticMesh->GetStaticMesh()->GetBodySetup();
	if (BodySetup)
	{
		bool isCollisionSetup = BodySetup->AggGeom.GetElementCount() > 0;
		if (isCollisionSetup&& !(TargetStaticMesh->IsSimulatingPhysics()))
		{
			UE_LOG(LogTemp, Log, TEXT("BarBaseActor : Enable StaticMesh Physics"));
			TargetStaticMesh->SetSimulatePhysics(true);
			TargetStaticMesh->SetEnableGravity(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BarBaseActor : Specify StaticMesh First"));
	}

	return TargetStaticMesh;
}

FVector ABarBaseActor::GetStaticMeshBoxUnitSize(UStaticMesh* TargetStaticMesh) const
{
	FVector BoundsExtent = TargetStaticMesh->GetBounds().BoxExtent;
	return BoundsExtent * 2.0f;
}

// 에디터 상에 Procedural Mesh 또는 커스텀 메시 생성 유무 bool로 추출해놓음 분기하여 메시 생성 함수 결정
void ABarBaseActor::CreateMesh(float BarHeight)
{
	if (isProceduralMeshUsing)
	{
		CreateProceduralBoxMesh(BarHeight);
	}
	else
	{
		if (CustomStaticMeshComponent)
		{
			UE_LOG(LogTemp, Log, TEXT("BarBaseActor : CreateMesh : Create Custom Static Mesh"));
			CreateCustomMesh(BarHeight);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BarBaseActor : CreateMesh : Specify Custom Static Mesh First"));
		}
	}
}

// 라벨 텍스트 렌더러 설정
void ABarBaseActor::InitializeTextMeshLabel(const FString& LabelName)
{

	// 텍스트 내용
	TextRenderComponentLabel->SetText(FText::FromString(LabelName));
}

// 값 텍스트 렌더러 설정
void ABarBaseActor::InitializeTextMeshValue(const float& FloatValue, const float& BarHeight)
{
	int padding = 10;
	
	// 텍스트 내용
	TextRenderComponentValue->SetText(FText::AsNumber(FloatValue));

	// 위치
	TextRenderComponentValue->AddWorldOffset(FVector(0.f, 0.f, BarHeight + (TextRenderComponentValue->WorldSize)/2 
		+ padding));
}

// 애니메이션 실행 제어
void ABarBaseActor::PlayBarAnimation()
{
	if (AnimationCurve)
	{
		FOnTimelineFloat TimelineCallBack;
		// 타임라인에 함수 바인딩
		TimelineCallBack.BindUFunction(this, FName("OnAnimationUpdate"));

		// Timeline에 Curve와 Curve를 사용할 Callback 함수 추가
		BarAnimationTimeline->AddInterpFloat(AnimationCurve, TimelineCallBack);
		BarAnimationTimeline->SetLooping(false);
		BarAnimationTimeline->PlayFromStart();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BarBaseActor : Failed finding Animation Cuvrve"));
	}
}

// 막대 스케일 변경 애니메이션 실행
void ABarBaseActor::OnAnimationUpdate(float Value)
{
	//UE_LOG(LogTemp, Log, TEXT("BarBaseActor : BarAnimation Executing, Current Height : %f"), Value);

	FVector CurrentScale = GetActorScale();
	SetActorScale3D(FVector(CurrentScale.X, CurrentScale.Y, Value));
}



