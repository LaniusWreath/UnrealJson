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
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bEnableSpawnCustomMesh"), Category = "JCM")
	bool bUseStaticMeshInventory;

	// Add Static Mesh to Spawn, you have to match
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUseStaticMeshInventory"), Category = "JCM")
	TArray<UStaticMesh*> StaticMeshComponentInventory;

};
