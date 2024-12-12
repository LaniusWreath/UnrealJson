// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/JCMInventoryBarBaseActor.h"
#include "Datas/JCMLog.h"
#include "Datas/StaticMeshInventory.h"


AJCMInventoryBarBaseActor::AJCMInventoryBarBaseActor()
{
	TemplateItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InventoryItemStaticMeshComponent"));
	TemplateItemMeshComponent->SetupAttachment(CustomStaticMeshTemplateComponent);
}

// 입력받은 스태틱 메쉬 컴포넌트에 인벤토리 메쉬 적용
void AJCMInventoryBarBaseActor::InitializeStaticMeshPropertyFromInventory(UStaticMeshComponent* TargetStaticMeshComponent, const int32 InInventoryIndex)
{
	UStaticMesh* TemplateStaticMesh = GetStaticMeshFromInventory(InInventoryIndex);

	if (!TemplateStaticMesh)
	{
		UE_LOG(JCMlog, Error, TEXT("%s : Invalid StaticMesh"), *this->GetActorLabel());
		return;
	}

	TargetStaticMeshComponent->SetStaticMesh(TemplateStaticMesh);
}

// 인벤토리에서 스태틱 메쉬 가져오기
UStaticMesh* AJCMInventoryBarBaseActor::GetStaticMeshFromInventory(const int32 InInventoryIndex)
{
	if (!InventoryDataAsset)
	{
		UE_LOG(JCMlog, Error, TEXT("%s : Inventory Data Asset is invalid"), *this->GetActorLabel());
		return nullptr;
	}

	if (!InventoryDataAsset->GetStaticMesh(InInventoryIndex))
	{
		UE_LOG(JCMlog, Error, TEXT("%s : Inventory member is invalid"), *this->GetActorLabel());
		return nullptr;
	}

	return InventoryDataAsset->GetStaticMesh(InInventoryIndex);
}

void AJCMInventoryBarBaseActor::InitializeItemStaticMeshPrepertyFromTemplate(UStaticMeshComponent* TargetStaticMeshComponent,
	const int32 InventoryIndex)
{
	if (!TemplateItemMeshComponent)
	{
		UE_LOG(JCMlog, Error, TEXT("TemplateItemStaticMeshComponent is Invalid"));
		return;
	}

	// 스태틱 메쉬 적용
	TargetStaticMeshComponent->SetStaticMesh(GetStaticMeshFromInventory(InventoryIndex));

	// 스태틱 메쉬 컴포넌트 스케일링
	TargetStaticMeshComponent = ScaleStaticMeshToTemplateBounds(TargetStaticMeshComponent, TemplateItemMeshComponent);

	// 섀도우 
	TargetStaticMeshComponent->SetCastShadow(TemplateItemMeshComponent->CastShadow);
}

// 개수 만큼 커스텀 메쉬 컴포넌트 생성
void AJCMInventoryBarBaseActor::CreateSingleCustomMeshComponent(const float BarHeight, const float UnitMeshHeight, int32 SpawnAmount)
{
	// 스폰 카운트 제한 체크, 
	if (SpawnCount >= SpawnAmount)
	{
		ClearSpawnTimerHandle();
	}
	else
	{
		// 커버 스태틱 메쉬 컴포넌트를 동적으로 생성하고, 부모 액터에 속하도록 설정
		UStaticMeshComponent* CoverMeshComponent = NewObject<UStaticMeshComponent>(this);

		// 아이템 스태틱 메쉬 컴포넌트를 동적으로 생성하고, 부모 액터에 속하도록 설정
		UStaticMeshComponent* ItemMeshComponent = NewObject<UStaticMeshComponent>(this);

		// 템플릿으로부터 커버 메쉬, 머티리얼, 스케일 복사
		InitializeStaticMeshProperty(CoverMeshComponent, CustomStaticMeshTemplateComponent);

		// 템플릿으로부터 아이템 메쉬, 스케일 초기화
		InitializeItemStaticMeshPrepertyFromTemplate(ItemMeshComponent, SpawnCount);

		if (bEnablePhysics)
		{
			// 템플릿으로부터 피직스 복사
			InitializeCustomStaticMeshPhysics(CoverMeshComponent, CustomStaticMeshTemplateComponent);
		}

		// 아이템 커버에 부착
		ItemMeshComponent->AttachToComponent(CoverMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);

		CoverMeshComponent->AttachToComponent(CustomActorSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

		// Z축 스폰 오프셋 조정
		if (SpawnPerUnitValue)
		{
			CoverMeshComponent->AddWorldOffset(FVector(0, 0, UnitMeshHeight * SpawnAmount + UnitMeshHeight / 2));
		}
		else
		{
			CoverMeshComponent->AddWorldOffset(FVector(0, 0, BarHeight + UnitMeshHeight * 3));
		}

		// 월드에 컴포넌트를 등록하여 액터와 함께 관리되도록 설정 
		ItemMeshComponent->RegisterComponent();
		CoverMeshComponent->RegisterComponent();

		// 스폰 카운트
		SpawnCount++;
	}
}

// 스태틱 메쉬 컴포넌트 하나만 생성
void AJCMInventoryBarBaseActor::CreateSingleCustomMeshComponent(const float UnitMeshHeight)
{
	// StaticMeshComponent를 동적으로 생성하고, 부모 액터에 속하도록 설정
	UStaticMeshComponent* CoverMeshComponent = NewObject<UStaticMeshComponent>(this);

	// 아이템 스태틱 메쉬 컴포넌트를 동적으로 생성하고, 부모 액터에 속하도록 설정
	UStaticMeshComponent* ItemMeshComponent = NewObject<UStaticMeshComponent>(this);

	// 템플릿으로부터 커버 메쉬, 머티리얼, 스케일 복사
	InitializeStaticMeshProperty(CoverMeshComponent, CustomStaticMeshTemplateComponent);

	// 템플릿으로부터 아이템 메쉬, 스케일 초기화
	InitializeItemStaticMeshPrepertyFromTemplate(ItemMeshComponent, ParentSplineIndex);

	if (bEnablePhysics)
	{
		// 템플릿으로부터 피직스 복사
		InitializeCustomStaticMeshPhysics(CoverMeshComponent, CustomStaticMeshTemplateComponent);
	}

	// 아이템 커버에 부착
	ItemMeshComponent->AttachToComponent(CoverMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// 커버 루트에 부착
	CoverMeshComponent->AttachToComponent(CustomActorSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Z축 스폰 오프셋 조정
	CoverMeshComponent->AddWorldOffset(FVector(0, 0, UnitMeshHeight / 2));

	CoverMeshComponent->RegisterComponent();
	ItemMeshComponent->RegisterComponent();

}


// 템플릿 스태틱 메쉬 컴포넌트의 바운딩 크기 만큼 타겟 컴포넌트 스케일링
UStaticMeshComponent* AJCMInventoryBarBaseActor::ScaleStaticMeshToTemplateBounds(UStaticMeshComponent* NewMesh, UStaticMeshComponent* TemplateMesh)
{
	if (!NewMesh || !NewMesh->GetStaticMesh() || !TemplateMesh || !TemplateMesh->GetStaticMesh())
	{
		UE_LOG(JCMlog, Warning, TEXT("Invalid Mesh Components"));
		return nullptr;
	}

	// 템플릿 메쉬 바운드
	FBoxSphereBounds TemplateBounds = TemplateMesh->GetStaticMesh()->GetBounds();

	// 새 컴포넌트의 바운드
	FBoxSphereBounds NewBounds = NewMesh->GetStaticMesh()->GetBounds();

	// 템플릿과 타겟 컴포넌트의 최대 크기 비교
	float TemplateMaxSize = FMath::Max3(TemplateBounds.BoxExtent.X, TemplateBounds.BoxExtent.Y, TemplateBounds.BoxExtent.Z) * 2.0f;
	float NewMaxSize = FMath::Max3(NewBounds.BoxExtent.X, NewBounds.BoxExtent.Y, NewBounds.BoxExtent.Z) * 2.0f;

	// 크기 비교 계산
	if (NewMaxSize > 0.0f)
	{
		float ScaleFactor = TemplateMaxSize / NewMaxSize;
		NewMesh->SetRelativeScale3D(FVector(ScaleFactor));
		return NewMesh;
	}
	else
	{
		UE_LOG(JCMlog, Warning, TEXT("New mesh bounds are invalid."));
		return nullptr;
	}
}