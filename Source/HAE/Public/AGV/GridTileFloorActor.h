// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AGVDataTypes.h"
#include "GridTileFloorActor.generated.h"

UCLASS()
class HAE_API AGridTileFloorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridTileFloorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Generating GridTile
	void GenerateGrid(int32 InRows, int32 InColumns, float InTileSize);

	// Get Tile Data
	FTile* GetTileAt(const int32 InRow, const int32 InColumn);
	FIntPoint GetWorldLocationFromTile(const FVector& InWorldLocation, float InTileSize);

	// Call Generating GridTile Function
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	
	// Update Mesh Scale
	void UpdateGridMesh(const int32 InNumRows, const int32 InNumColumns, const float TileSize);

	// Update Mesh Material
	void UpdateTileVisuals(int32 Row, int32 Column);

private:
// Grid Data
	// Editable Accessable or Denied tiles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, FTile> Tiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	int32 NumRows =10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	int32 NumColumns = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	float TileSize = 100.0f;  // 타일 한 변의 크기

// Mesh & Material
	// Grid StaticMesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> GridMesh;
};
