// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/JCMInventoryBarBaseActor.h"
#include "Datas/JCMLog.h"


AJCMInventoryBarBaseActor::AJCMInventoryBarBaseActor()
{
	InventoryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InventoryItemStaticMeshComponent"));
	InventoryMesh->SetupAttachment(CustomStaticMeshTemplateComponent);
}

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

UStaticMesh* AJCMInventoryBarBaseActor::GetStaticMeshFromInventory(const int32 InInventoryIndex)
{
	if (!StaticMeshComponentInventory[InInventoryIndex])
	{
		UE_LOG(JCMlog, Warning, TEXT("%s : Invalid Inventory Index"), *this->GetActorLabel())
			return CustomStaticMeshTemplateComponent->GetStaticMesh();
	}

	return StaticMeshComponentInventory[InInventoryIndex];
}
