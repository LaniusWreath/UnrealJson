// Fill out your copyright notice in the Description page of Project Settings.

#include "AGV/WheelMovementComponent.h"
#include "AGV/AGVDataContainer.h"

// Sets default values for this component's properties
UWheelMovementComponent::UWheelMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DataContainerRef = nullptr;
	// ...
}


void UWheelMovementComponent::UpdateMovement(float DeltaTime)
{
	/*if (!WheelDataContainer) return;

	FAGVData Data = WheelDataContainer->GetAGVData();*/

	// 운동 로직 처리 (예: 휠의 회전 및 이동)
}

// Called when the game starts
void UWheelMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWheelMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

