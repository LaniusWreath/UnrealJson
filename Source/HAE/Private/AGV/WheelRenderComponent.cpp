// Fill out your copyright notice in the Description page of Project Settings.

#include "AGV/WheelRenderComponent.h"
#include "Components/StaticMeshComponent.h"

UWheelRenderComponent::UWheelRenderComponent()
{
    ChassisMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chassis"));
    //ChassisMeshComponent->SetupAttachment(this);
}

void UWheelRenderComponent::SetWheelMesh(UStaticMesh* Mesh)
{
    ChassisMeshComponent->SetStaticMesh(Mesh);
    // �߰� ���� ���� : ��Ƽ����, �ݸ��� �� 
}

void UWheelRenderComponent::BeginPlay()
{
}
