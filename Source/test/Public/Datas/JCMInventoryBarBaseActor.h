// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Datas/JCMBarBaseActor.h"
#include "JCMInventoryBarBaseActor.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API AJCMInventoryBarBaseActor : public AJCMBarBaseActor
{
	GENERATED_BODY()
	
public:
	AJCMInventoryBarBaseActor();

	// Set StaticMesh in StaticMeshComponent with Mesh Inventory
	void InitializeStaticMeshPropertyFromInventory(UStaticMeshComponent* TargetStaticMeshComponent,
		const int32 InInventoryIndex);

	// Get StaticMesh from Inventory
	UStaticMesh* GetStaticMeshFromInventory(const int32 InInventoryIndex);

	// Creating Mesh
	void CreateSingleCustomMeshComponent(const float BarHeight, const float UnitMeshHeight, int32 SpawnAmount) override;
	// Creating Mesh Only One
	void CreateSingleCustomMeshComponent(const float UnitMeshHeight) override;

	void InitializeItemStaticMeshPrepertyFromTemplate(UStaticMeshComponent* TargetStaticMeshComponent, const int32 InventoryIndex);
	
	// Scaling Target Mesh Bounds same as TemplateMesh Bounds
	UStaticMeshComponent* ScaleStaticMeshToTemplateBounds(UStaticMeshComponent* NewMesh, UStaticMeshComponent* TemplateMesh);
	
	// 회전 타이머 함수
	void InitializeItemMeshRotation(UStaticMeshComponent* TargetStaticMeshComponent, const float InRotationSpeed);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUseStaticMeshInventory"), Category = "JCM")
	TObjectPtr<class UStaticMeshInventory> InventoryDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bEnableItemRotation"), Category = "JCM")
	float RotationSpeed = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUseStaticMeshInventory"), Category = "JCM")
	bool bEnableItemRotation = false;

private:

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bEnableSpawnCustomMesh", AllowPrivateAccess = "true"), Category = "JCM")
	bool bUseStaticMeshInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUseStaticMeshInventory", AllowPrivateAccess = "true"), Category = "JCM")
	TObjectPtr<UStaticMeshComponent> TemplateItemMeshComponent;

};
