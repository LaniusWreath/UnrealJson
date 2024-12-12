// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/StaticMeshInventory.h"
#include "Datas/JCMLog.h"

TArray<UStaticMesh*> UStaticMeshInventory::GetStaticMeshArray()
{
	if (StaticMeshArray.Num() ==0)
	{
		UE_LOG(JCMlog, Warning, TEXT("StaticMeshInventory size is 0"));
	}

	return StaticMeshArray;
}

UStaticMesh* UStaticMeshInventory::GetStaticMesh(const int32 InventoryIndex)
{
	if (!StaticMeshArray[InventoryIndex])
	{
		UE_LOG(JCMlog, Error, TEXT("StaticMeshInventory : Invalid Inventory Index"));
		return nullptr;
	}

	return StaticMeshArray[InventoryIndex];
}
