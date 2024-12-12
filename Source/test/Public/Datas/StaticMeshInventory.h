// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StaticMeshInventory.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TEST_API UStaticMeshInventory : public UDataAsset
{
	GENERATED_BODY()
	

private:
	// StaticMesh Array
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "JCM")
	TArray<UStaticMesh*> StaticMeshArray;

public:
	TArray<UStaticMesh*> GetStaticMeshArray();

	UStaticMesh* GetStaticMesh(const int32 InventoryIndex);
};
