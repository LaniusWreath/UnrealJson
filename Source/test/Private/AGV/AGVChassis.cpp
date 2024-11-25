// Fill out your copyright notice in the Description page of Project Settings.

#include "AGV/AGVChassis.h"
#include "AGV/AGVLog.h"
#include "AGV/WheelMovementComponent.h"
#include "AGV/WheelRenderComponent.h"
#include "AGV/AGVDataContainer.h"

// Sets default values
AAGVChassis::AAGVChassis()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Wheel Render Component 积己
    WheelRenderComponent = CreateDefaultSubobject<UWheelRenderComponent>(TEXT("WheelRenderComponent"));
    WheelRenderComponent->SetupAttachment(RootComponent);
    WheelRenderComponent->ChassisMeshComponent->SetupAttachment(WheelRenderComponent);

    // Wheel Movement Component 积己
    WheelMovementComponent = CreateDefaultSubobject<UWheelMovementComponent>(TEXT("WheelMovementComponent"));
    WheelMovementComponent->SetupAttachment(RootComponent);

    // Wheel Data Container甫 Movement俊 楷搬
    WheelMovementComponent->DataContainerRef = WheelDataContainer;
}

// Called when the game starts or when spawned
void AAGVChassis::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAGVChassis::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

