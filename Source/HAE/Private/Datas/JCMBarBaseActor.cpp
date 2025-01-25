// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/JCMBarBaseActor.h"
#include "ProceduralMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/TextRenderComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "PhysicsEngine/BodySetup.h"
#include "PhysicsEngine/BodyInstance.h"
#include "TimerManager.h"
#include "Datas/JCMLog.h"
#include "Datas/JCMChartGenerator.h"


// Sets default values
AJCMBarBaseActor::AJCMBarBaseActor()
{
	DefaultSceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = DefaultSceneRootComponent;

	ProcMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("BasicMesh"));
	ProcMeshComponent->SetupAttachment(RootComponent);

	CustomActorSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomMeshOffsetLayer"));
	CustomActorSceneComponent->SetupAttachment(RootComponent);

	CustomStaticMeshTemplateComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomMesh"));
	CustomStaticMeshTemplateComponent->SetupAttachment(RootComponent);

	//CustomStaticMeshTemplateComponent->SetHiddenInGame(true);

	// Procedural Mesh Component�� Navigation �ý��ۿ��� ����. ���Ž���̳� ai��ȣ�ۿ��� �ʿ� ���� ���, ���� ����. 
	// �Ȳ��� Navigation system���� ȭ��. (��� ������)
	ProcMeshComponent->SetCanEverAffectNavigation(false);
	BarAnimationTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("BarAnimationTimeline"));

	// �ؽ�Ʈ ������ - ��
	TextRenderComponentValue = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponentValue"));
	TextRenderComponentValue->SetupAttachment(RootComponent);

	// �ؽ�Ʈ �ǹ��� ����� ����
	TextRenderComponentValue->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRenderComponentValue->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	
	// �ؽ�Ʈ ������ - ��
	TextRenderComponentLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponentLabel")); 
	TextRenderComponentLabel->SetupAttachment(RootComponent);

	TextRenderComponentLabel->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRenderComponentLabel->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);

}


// Called when the game starts or when spawned
void AJCMBarBaseActor::BeginPlay()
{
	Super::BeginPlay();

	// ���ø����� �� Ŀ���� ����ƽ �޽��� �ݸ��� ����
	CustomStaticMeshTemplateComponent->SetSimulatePhysics(false);
	CustomStaticMeshTemplateComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// ���ø����� �� Ŀ���� ����ƽ �޽ô� ������ ������ ǥ��/���� �����ؾ� ��.

}

void AJCMBarBaseActor::CreateProceduralBoxMesh(float BarHeight)
{
	// �ʿ��� �迭 ����
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> Tangents;

	float BarWidth = Width_bar;
	float w = BarWidth / 2;

	// �ڽ��� ũ�� ���� (�ڽ� �ʺ�)
	FVector BoxRadius(w, w, w); // �ڽ��� ���� ũ��

	// �ڽ� �޽� ����
	UKismetProceduralMeshLibrary::GenerateBoxMesh(BoxRadius, Vertices, Triangles, Normals, UV0, Tangents);

	// �ڽ� ���̸�ŭ ������ ����ؼ� ���� �ø�.
	float zScaler = BarHeight / (BarWidth);
	//UE_LOG(LogTemp, Log, TEXT("BarBaseActor : zScaler is %f"), zScaler);
	ProcMeshComponent->SetWorldScale3D(FVector(1.f, 1.f, zScaler));
	ProcMeshComponent->AddWorldOffset(FVector(0, 0, (BarHeight / 2)));

	// Procedural Mesh Component�� �޽� ����
	ProcMeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, {}, Tangents, true);

	if (MeshMaterial)
	{
		ProcMeshComponent->SetMaterial(0, MeshMaterial);
	}
}

// Ŀ���� ����ƽ �޽� ���� ��ƾ : ���� �ڵ� ���
void AJCMBarBaseActor::CreateCustomMeshRoutine(float BarHeight)
{
	// Ŀ���� �޽� ���� ���� : ���� ���� * ���� �����Ϸ�
	float UnitMeshHeight = GetStaticMeshBoxUnitSize(CustomStaticMeshTemplateComponent->GetStaticMesh()).Z * 
		CustomStaticMeshTemplateComponent->GetRelativeScale3D().Z;

	// �����ؾ��ϴ� �޽� ���� : 
	int32 UnitMeshAmount;
	UnitMeshAmount = BarHeight / UnitMeshHeight;

	// Ÿ�̸� ����, ���ٷ� �Ű����� �ִ� �Լ� ĸ��
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, [this, BarHeight, UnitMeshHeight, UnitMeshAmount]()
	{
		CreateSingleCustomMeshComponent(BarHeight, UnitMeshHeight, UnitMeshAmount); 
	}, CustomMeshSpawnWaitingTime, true);
}

// ������ŭ ���� ��Ʈ �޽� ����
void AJCMBarBaseActor::CreateCustomMeshRoutine(float BarHeight, int32 amount)
{
	// Ŀ���� �޽� ���� ���� : ���� ���� * ���� �����Ϸ�
	float UnitMeshHeight = GetCustomMeshUnitHeight();

	// �����ؾ��ϴ� �޽� ���� : 
	int32 UnitMeshAmount = amount;

	// Ÿ�̸� ����, ���ٷ� �Ű����� �ִ� �Լ� ĸ��
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, [this, BarHeight, UnitMeshHeight, UnitMeshAmount]()
		{
			CreateSingleCustomMeshComponent(BarHeight, UnitMeshHeight, UnitMeshAmount);
		}, CustomMeshSpawnWaitingTime, true);
}

// �ϳ��� �����ϴ� ��ƾ �Լ�
void AJCMBarBaseActor::CreateCustomMeshRoutine()
{
	// Ŀ���� �޽� ���� ���� : ���� ���� * ���� �����Ϸ�
	float UnitMeshHeight = GetCustomMeshUnitHeight();

	CreateSingleCustomMeshComponent(UnitMeshHeight);
}

// �ڽ����� �޽� ���� ������ ����
FVector AJCMBarBaseActor::GetStaticMeshBoxUnitSize(UStaticMesh* TargetStaticMesh) const
{
	FVector BoundsExtent = TargetStaticMesh->GetBounds().BoxExtent;
	return BoundsExtent * 2.0f;
}

// Ŀ���� ����ƽ �޽� ���� �Լ�
void AJCMBarBaseActor::CreateSingleCustomMeshComponent(const float BarHeight, const float UnitMeshHeight, 
	const int32 SpawnAmount)
{
	// ���� ī��Ʈ ���� üũ, 
	if (SpawnCount >= SpawnAmount)
	{
		ClearSpawnTimerHandle();
	}
	else
	{
		// StaticMeshComponent�� �������� �����ϰ�, �θ� ���Ϳ� ���ϵ��� ����
		UStaticMeshComponent* UnitMeshComponent = NewObject<UStaticMeshComponent>(this);

		// ���ø� ����ƽ �޽� ������Ʈ�κ��� �޽�, ��Ƽ����, ������ ����
		InitializeStaticMeshProperty(UnitMeshComponent, CustomStaticMeshTemplateComponent);
		

		if (bEnablePhysics)
		{
			// ���ø����κ��� ������ ����
			InitializeCustomStaticMeshPhysics(UnitMeshComponent, CustomStaticMeshTemplateComponent);
		}
		
		// ����
		UnitMeshComponent->AttachToComponent(CustomActorSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

		// Z�� ���� ������ ����
		if (SpawnPerUnitValue)
		{
			UnitMeshComponent->AddWorldOffset(FVector(0, 0, UnitMeshHeight * SpawnAmount + UnitMeshHeight/2));
		}
		else 
		{
			UnitMeshComponent->AddWorldOffset(FVector(0, 0, BarHeight + UnitMeshHeight * 3));
		}

		// ���忡 ������Ʈ�� ����Ͽ� ���Ϳ� �Բ� �����ǵ��� ���� 
		UnitMeshComponent->RegisterComponent();

		// ���� ī��Ʈ
		SpawnCount++;
	}
}

// �޽� �ϳ��� �����ϴ� �Լ�
void AJCMBarBaseActor::CreateSingleCustomMeshComponent(const float UnitMeshHeight)
{
	// StaticMeshComponent�� �������� �����ϰ�, �θ� ���Ϳ� ���ϵ��� ����
	UStaticMeshComponent* UnitMeshComponent = NewObject<UStaticMeshComponent>(this);

	// �κ��丮�� �ƴ� ���ø� ����ƽ �޽� ������Ʈ�κ��� �޽�, ��Ƽ����, ������ ����
	InitializeStaticMeshProperty(UnitMeshComponent, CustomStaticMeshTemplateComponent);
	
	if (bEnablePhysics)
	{
		// ���ø����κ��� ������ ����
		InitializeCustomStaticMeshPhysics(UnitMeshComponent, CustomStaticMeshTemplateComponent);
	}

	// ����
	UnitMeshComponent->AttachToComponent(CustomActorSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Z�� ���� ������ ����
	UnitMeshComponent->AddWorldOffset(FVector(0, 0, UnitMeshHeight / 2));

	UnitMeshComponent->RegisterComponent();
}

// ���� ������ ����ƽ �޽� ������Ʈ ����
void AJCMBarBaseActor::ClearCustomMeshes()
{
	if (CustomActorSceneComponent->GetNumChildrenComponents() > 0)
	{
		TArray<USceneComponent*> AttachedComponents;
		CustomActorSceneComponent->GetChildrenComponents(false, AttachedComponents);

		for (USceneComponent* ChildComponent : AttachedComponents)
		{
			if (UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(ChildComponent))
			{
				MeshComponent->DestroyComponent();
			}
		}
		UE_LOG(JCMlog, Log, TEXT("%s : All customStaticmesh components cleard"), *this->GetAttachParentActor()->GetName());
	}
}

// �޽� ���� Ÿ�̸� �ڵ� �ʱ�ȭ : BarBaseActor�� BarGenerator���� Gernating �� �� ���� ���ο� �ν��Ͻ��� �����Ǿ� �����.
// ���� ���� BarBaseActor�� �޽� ���� Ÿ�̸Ӹ� ������Ű�� ���� ���� �ν��Ͻ��� �����ϰ� �ִ� BarGenerator���� ���� ������� ��.
void AJCMBarBaseActor::ClearSpawnTimerHandle()
{
	if (GetWorldTimerManager().IsTimerActive(SpawnTimerHandle))
	{
		// Ÿ�̸� ����
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

		// Ÿ�̸� ����� ȣ���� �Լ��� ���⿡ �ۼ�
	}
}

void AJCMBarBaseActor::InitializeStaticMeshProperty(UStaticMeshComponent* TargetStaticMeshComponent, 
	const UStaticMeshComponent* TemplateMeshComponent)
{
	// ���ø��� �Ӽ��� UnitMeshComponent�� ����
	TargetStaticMeshComponent->SetStaticMesh(TemplateMeshComponent->GetStaticMesh());
	TargetStaticMeshComponent->SetMaterial(0, TemplateMeshComponent->GetMaterial(0));
	TargetStaticMeshComponent->SetRelativeScale3D(TemplateMeshComponent->GetRelativeScale3D());
	TargetStaticMeshComponent->SetCastShadow(false);
}

// ������ ������ ������ ���� ����, �����ϸ� �� ���� ���� ����� ���� : ����� �Ⱦ�
void AJCMBarBaseActor::CreateAdditionalCustomMeshComponent(float BarHeight, float RestHeight, float UnitMeshHeight)
{	
	// StaticMeshComponent�� �������� �����ϰ�, �θ� ���Ϳ� ���ϵ��� ����
	UStaticMeshComponent* RestMeshComponent = NewObject<UStaticMeshComponent>(this);

	// ���� ���� ����
	InitializeCustomStaticMeshPhysics(RestMeshComponent, CustomStaticMeshTemplateComponent);

	// ������ ����
	float scaler = RestHeight / UnitMeshHeight;
	RestMeshComponent->SetRelativeScale3D(RestMeshComponent->GetRelativeScale3D() * FVector(1, 1, scaler));

	// ����
	RestMeshComponent->AttachToComponent(CustomActorSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Z�� ���� ������ ����
	RestMeshComponent->AddWorldOffset(FVector(0, 0, BarHeight + UnitMeshHeight * 3));

	// ���忡 ������Ʈ�� ����Ͽ� ���Ϳ� �Բ� �����ǵ��� ���� 
	RestMeshComponent->RegisterComponent();
}

// ���ø� �޽��κ��� �Ӽ� ����
void AJCMBarBaseActor::InitializeCustomStaticMeshPhysics(UStaticMeshComponent* TargetStaticMesh, UStaticMeshComponent* TemplateComponent)
{
	// ������ ���� : BodyInstance�� ���� ������ �� ������, �� ��쿡�� AttachToComponent�� ����� �� ����. -> ���� ���� �ʿ�
	TargetStaticMesh->SetSimulatePhysics(true); // �ùķ��̼� ���� ����
	TargetStaticMesh->SetEnableGravity(TemplateComponent->IsGravityEnabled()); // �߷� on
	TargetStaticMesh->SetLinearDamping(TemplateComponent->GetLinearDamping()); // ���� ����
	TargetStaticMesh->SetAngularDamping(TemplateComponent->GetAngularDamping()); // ȸ���� ����
	//UnitMeshComponent->SetPhysMaterialOverride(CustomStaticMeshComponent->GetBodySetup()->GetPhysMaterial()); // ������ ��Ƽ����
	// ������ ��Ƽ������ �ڿ� ���� ����, �����ϴµ��� ���� �ڿ� �ʿ��ϴ�, ����
	TargetStaticMesh->BodyInstance.SetMassOverride(TemplateComponent->BodyInstance.GetMassOverride()); //����

	// �ݸ��� ���� : �ݸ���-�������� bodyinstance�� ���� ���� ��. bodyinstance ��� ���ϴ� ����, �ݸ����� ������ ����
	TargetStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TargetStaticMesh->SetCollisionObjectType(TemplateComponent->GetCollisionObjectType());
	TargetStaticMesh->SetCollisionResponseToChannels(TemplateComponent->GetCollisionResponseToChannels());

	// ����Ʈ����Ʈ ����
	TargetStaticMesh->BodyInstance.bLockRotation = TemplateComponent->BodyInstance.bLockRotation;
	TargetStaticMesh->BodyInstance.bLockTranslation = TemplateComponent->BodyInstance.bLockTranslation;

	// ��ݷ� �ݿ� ����
	TargetStaticMesh->bApplyImpulseOnDamage = TemplateComponent->bApplyImpulseOnDamage;
}

void AJCMBarBaseActor::AdjustTextMeshComponentOffsetRoutine(const float InHeight)
{
	AdjustTextMeshValueOffset(TextRenderComponentValue, InHeight); // �ؽ�Ʈ ���� ������Ʈ ������ ����
	AdjustTextMeshOffset(TextRenderComponentLabel);
}


// ������ �� Procedural Mesh �Ǵ� Ŀ���� �޽� ���� ���� bool�� �����س��� �б��Ͽ� �޽� ���� �Լ� ����
void AJCMBarBaseActor::CreateMesh(float BarHeight, int Value)
{
	// ���ν����� �޽�
	if (!bEnableSpawnCustomMesh)
	{
		CreateProceduralBoxMesh(BarHeight);	// �޽� ����
		AdjustTextMeshComponentOffsetRoutine(BarHeight); // �ؽ�Ʈ ���� ������Ʈ ������ ���� �Լ��� ȣ��
	}
	// Ŀ���� �޽�
	else
	{
		if (!CustomStaticMeshTemplateComponent)
		{
			UE_LOG(JCMlog, Error, TEXT("CustomStaticMeshTemplateComponent is invalid"));
			return;
		}
		float UnitMeshHeight = GetCustomMeshUnitHeight();
		// ���� �����Ͽ� �޽� ����
		if (SpawnPerUnitValue)
		{
			// ������ ���! UnitValue�� 0�̸� ������ �ϳ��� ����
			if (UnitValue == 0)
			{
				SpawnedCustomMeshAmount = 1;
				CreateCustomMeshRoutine();
			}
			else
			{
				// ����� ���� ������ ���� 
				SpawnedCustomMeshAmount = int32(Value / UnitValue);
				CreateCustomMeshRoutine(BarHeight, SpawnedCustomMeshAmount);
			}
		}
		// ���� �ڵ� ����Ͽ� �޽� ����
		else
		{
			// �����ؾ��ϴ� �޽� ���� : 
			SpawnedCustomMeshAmount = int32(BarHeight / UnitMeshHeight);
			CreateCustomMeshRoutine(BarHeight);
		}

		// ������ ���ڰ� ǥ�� ������Ʈ ������ ���� �Լ��� ȣ��
		AdjustTextMeshComponentOffsetRoutine(SpawnedCustomMeshAmount * UnitMeshHeight);
	}
}

// �� �ؽ�Ʈ ������ ����
void AJCMBarBaseActor::InitializeTextMeshLabel(const FString& LabelName)
{
	// �ؽ�Ʈ ����
	TextRenderComponentLabel->SetText(FText::FromString(LabelName));
}

// �� �ؽ�Ʈ ������ ����
void AJCMBarBaseActor::InitializeTextMeshValue(const float& FloatValue)
{
	// �ؽ�Ʈ ����
	TextRenderComponentValue->SetText(FText::AsNumber(FloatValue));
}

// ��Ȯ�� ���̿� �ؽ�Ʈ ���� ������Ʈ ��ġ
void AJCMBarBaseActor::AdjustTextMeshValueOffset(UTextRenderComponent* TargetTextRednerComponent, const float& BarHeight)
{
	TargetTextRednerComponent->AddWorldOffset(FVector(0.f, 0.f, BarHeight + (TargetTextRednerComponent->WorldSize) / 2
			+ TextRenderComponentOffset_Value));
}

// Ŀ���Ҹ޽� ������ ������ ���� ���̿� �ؽ�Ʈ ���� ������Ʈ ��ġ
void AJCMBarBaseActor::AdjustTextMeshValueOffset(UTextRenderComponent* TargetTextRednerComponent, const int32& amount)
{
	// Ŀ���� �޽� ���� ���� : ���� ���� * ���� �����Ϸ�
	float UnitMeshHeight = GetCustomMeshUnitHeight();

	TargetTextRednerComponent->AddWorldOffset(FVector(0.f, 0.f, (UnitMeshHeight*amount + (TargetTextRednerComponent->WorldSize / 2)
		+ TextRenderComponentOffset_Value)));
}

// �߰� ���� ���� ���� �ؽ�Ʈ 
void AJCMBarBaseActor::AdjustTextMeshOffset(UTextRenderComponent* TargetTextRednerComponent)
{
	TargetTextRednerComponent->AddWorldOffset(FVector(0.f, 0.f, (TargetTextRednerComponent->WorldSize / 2)
		+ TextRenderComponentOffset_Value));
}

// ��Ʈ ���� ����� ���ε��� ��������Ʈ �Լ� 
void AJCMBarBaseActor::OnChartGeneratingDoneBindingRoutine()
{
	return;
}

void AJCMBarBaseActor::OnChartSearchingBindingRoutine(int32 InIndex)
{
	return;
}

void AJCMBarBaseActor::BindToChartSearching(FChartSearchingDelegate& Delegate)
{
	Delegate.AddUObject(this, &AJCMBarBaseActor::OnChartSearchingBindingRoutine);
}

void AJCMBarBaseActor::BindToChartGeneratingEnd(FChartGeneratingDoneDelegate& Delegate)
{
	Delegate.BindUObject(this, &AJCMBarBaseActor::OnChartGeneratingDoneBindingRoutine);
}

// �ִϸ��̼� ����
void AJCMBarBaseActor::BindTimelineAnimation()
{
	// �ִϸ��̼� �ʱ�ȭ
	if (AnimationCurve && !bEnableSpawnCustomMesh)
	{
		FOnTimelineFloat TimelineCallBack;
		// Ÿ�Ӷ��ο� �Լ� ���ε�
		TimelineCallBack.BindUFunction(this, FName("OnAnimationUpdate"));

		// Timeline�� Curve�� Curve�� ����� Callback �Լ� �߰�
		BarAnimationTimeline->AddInterpFloat(AnimationCurve, TimelineCallBack);
		BarAnimationTimeline->SetLooping(false);
		BarAnimationTimeline->Play();
	}
	else
	{
		if (!bEnableSpawnCustomMesh)
		{
			UE_LOG(JCMlog, Warning, TEXT("%s : Failed finding animation cuvrve"), *this->GetAttachParentActor()->GetName());
		}
	}
}

float AJCMBarBaseActor::GetCustomMeshUnitHeight()
{
	if (CustomStaticMeshTemplateComponent)
	{
		return GetStaticMeshBoxUnitSize(CustomStaticMeshTemplateComponent->GetStaticMesh()).Z *
			CustomStaticMeshTemplateComponent->GetRelativeScale3D().Z;
	}	
	else
	{
		UE_LOG(JCMlog, Warning, TEXT("%s : CustomStaticMesh is invalid"), *this->GetAttachParentActor()->GetName());
		return 1;
	}
}

// �ִϸ��̼� ���� ����
void AJCMBarBaseActor::PlayBarAnimation()
{
	if (bEnableSpawnCustomMesh)
	{
		return;
	}
	BarAnimationTimeline->PlayFromStart();
}

// ���� ������ ���� �ִϸ��̼� ����
void AJCMBarBaseActor::OnAnimationUpdate(float Value)
{
	FVector CurrentScale = GetActorScale();
	SetActorScale3D(FVector(CurrentScale.X, CurrentScale.Y, Value));
}

void AJCMBarBaseActor::SetParentSplineIndex(const int32 InIndex)
{
	ParentSplineIndex = InIndex;
}

// �ؽ�Ʈ ���� ������Ʈ�κ��� ���� �� int32�� �������� �Լ�
int32 AJCMBarBaseActor::GetValueFromTextRenderComponent(UTextRenderComponent* TargetTextRenderComponent)
{
	if (TargetTextRenderComponent->Text.IsEmpty())
	{
		UE_LOG(JCMlog, Warning, TEXT("SafeValue is Empty"));
		return 0;
	}

	FText ValueText = TargetTextRenderComponent->Text;
	FString ValueString = ValueText.ToString();

	if (!ValueString.IsNumeric()) // ���ڿ��� �������� Ȯ��
	{
		UE_LOG(JCMlog, Warning, TEXT("SafeValue is not number"));
		return 0;
	}

	int32 Value = FCString::Atoi(*ValueString); // FString�� int32�� ��ȯ
	return Value;
}

// ���ö��� �ε����� �ش��ϴ� ������ ������ ������ ����ƽ �޽� ������Ʈ ������
UStaticMeshComponent* AJCMBarBaseActor::GetCustomStaticMeshComponent(const int32 SplineIndex)
{
	// RootSceneComponent�� �ڽ� ������Ʈ ��������
	USceneComponent* AttachedChildComponent = CustomActorSceneComponent->GetChildComponent(SplineIndex);

	// UStaticMeshComponent���� Ȯ��
	if (UStaticMeshComponent* TargetStaticMeshComponent = Cast<UStaticMeshComponent>(AttachedChildComponent))
	{
		return TargetStaticMeshComponent;
	}
	else
	{
		UE_LOG(JCMlog, Error, TEXT("Invalid CustomActorSceneComponent Index"))
		return nullptr;
	}
}

