// Fill out your copyright notice in the Description page of Project Settings.


#include "AGV/GridTileFloorActor.h"
#include "AGV/AGVLog.h"

// Sets default values
AGridTileFloorActor::AGridTileFloorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // 스태틱 메쉬 컴포넌트 초기화
    GridMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
    GridMesh->SetupAttachment(RootComponent);

    // 기본 스태틱 메쉬 설정
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Plane"));
    if (MeshAsset.Succeeded()) 
    {
        GridMesh->SetStaticMesh(MeshAsset.Object);
    }
}

// Called when the game starts or when spawned
void AGridTileFloorActor::BeginPlay()
{
	Super::BeginPlay();
    // 다이나믹 메쉬 머티리얼 초기화 함수 콜
    // InitializeDynamicMeshMaterial();
}

#if WITH_EDITOR
void AGridTileFloorActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    // 에디터에서 액터를 배치할 때 그리드 생성
    GenerateGrid(NumRows, NumColumns, TileSize);
    UpdateGridMesh(NumRows, NumColumns, TileSize);
}

// 액터 속성 변경 시 
void AGridTileFloorActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // 속성이 변경될 때 그리드 재생성
    GenerateGrid(NumRows, NumColumns, TileSize);
    UpdateGridMesh(NumRows, NumColumns, TileSize);
}
#endif

void AGridTileFloorActor::UpdateGridMesh(const int32 InNumRows, const int32 InNumColumns, const float InTileSize )
{
    FVector GridScale = FVector(InNumRows * InTileSize / 100.0f, InNumColumns * InTileSize / 100.0f, 1.0f);
    GridMesh->SetWorldScale3D(GridScale);

    FVector GridLocation = FVector((NumRows * InTileSize) / 2.0f, (InNumColumns * InTileSize) / 2.0f, 0.0f);
    GridMesh->SetWorldLocation(GridLocation);
}

void AGridTileFloorActor::UpdateTileVisuals(const int32 InRow, const int32 InColumn, bool bIsAccessible, bool bIsOccupied)
{
    if (!GridMeshDynamicMaterial)
    {
        UE_LOG(AGVlog, Warning, TEXT("UpdateTileVisual : DynamicMaterial is invalid"));
        return;
    }

    if (InRow < 0 || InColumn < 0)
    {
        UE_LOG(AGVlog, Warning, TEXT("UpdateTileVisual : Row or Column is invalid"));
        return;
    }

}

// 전체 타일에 적용할 할 다이나믹 머티리얼 생성
void AGridTileFloorActor::InitializeDynamicMeshMaterial()
{
    if (!GridMesh)
    {
        UE_LOG(AGVlog, Warning, TEXT("InitializeDynamicMeshMaterial : GridMesh is invalid"));
        return;
    }

    // 다이나믹 머티리얼 Create
    GridMeshDynamicMaterial = UMaterialInstanceDynamic::Create(GridMaterial, this);
    
    if (!GridMeshDynamicMaterial)
    {
        UE_LOG(AGVlog, Warning, TEXT("InitializeDynamicMeshMaterial : Creating DynamicMaterial Instance Failed"));
        return;
    }

    // 초기 파라미터 설정
    GridMeshDynamicMaterial->SetScalarParameterValue(TEXT("SnapTile"), 1000);
    GridMeshDynamicMaterial->SetVectorParameterValue(TEXT("GridColor"), FVector4(0.1f, 0.1f, 0.12f, 0.5f));

    // 머티리얼 적용
    GridMesh->SetMaterial(0, GridMeshDynamicMaterial);
}

// Called every frame
void AGridTileFloorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 그리드 타일 생성
void AGridTileFloorActor::GenerateGrid(int32 InRows, int32 InColumns, float InTileSize)
{
    // 기존 타일 제거
    Tiles.Empty();

    NumRows = InRows;
    NumColumns = InColumns;
    TileSize = InTileSize;

    for (int32 Row = 0; Row < NumRows; Row++)
    {
        for (int32 Column = 0; Column < NumColumns; Column++)
        {
            FTile NewTile;
            NewTile.bIsAccessible = true;
            NewTile.bIsOccupied = false;

            Tiles.Emplace(FIntPoint(Row, Column), NewTile);
        }
    }
}

// 특정 좌표의 타일 데이터 Get
FTile* AGridTileFloorActor::GetTileAt(const int32 InRow, const int32 InColumn)
{
    FIntPoint TileKey(InRow, InColumn);
    if(!Tiles.Contains(TileKey))
    {
        UE_LOG(AGVlog, Warning, TEXT("GetTileAt : %d, %d Tile is invalid"), InRow, InColumn);
        return nullptr;
    }

    return &Tiles[TileKey];
}

// 타일의 월드 좌표 Get
FIntPoint AGridTileFloorActor::GetWorldLocationFromTile(const FVector& InWorldLocation, const float InTileSize)
{
    int32 Row = FMath::FloorToInt(InWorldLocation.X / TileSize);
    int32 Column = FMath::FloorToInt(InWorldLocation.Y / TileSize);
    return FIntPoint(Row, Column);
}


