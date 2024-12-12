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

private:

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bEnableSpawnCustomMesh", AllowPrivateAccess = "true"), Category = "JCM")
	bool bUseStaticMeshInventory;

	// Add Static Mesh to Spawn, you have to match
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUseStaticMeshInventory", AllowPrivateAccess = "true"), Category = "JCM")
	TArray<UStaticMesh*> StaticMeshComponentInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUseStaticMeshInventory", AllowPrivateAccess = "true"), Category = "JCM")
	TObjectPtr<UStaticMeshComponent> InventoryMesh;

};
