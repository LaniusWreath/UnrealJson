// Fill out your copyright notice in the Description page of Project Settings.

#include "AGV/AGVVehiclePawn.h"
#include "AGV/AGVLog.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "ChaosVehicles/Public/ChaosVehicleWheel.h"
#include "ChaosVehicleMovementComponent.h"
#include "AGV/AGVDataContainer.h"

AAGVVehiclePawn::AAGVVehiclePawn()
{
	WheelFL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel FL"));
	WheelFL->SetupAttachment(RootComponent);

	WheelFR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel FR"));
	WheelFR->SetupAttachment(RootComponent);

	WheelRL =CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel RL"));
	WheelRL->SetupAttachment(RootComponent);

	WheelRR =CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel RR"));
	WheelRR->SetupAttachment(RootComponent);

}

void AAGVVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	// 섀시 스켈레탈 메쉬 소켓에 타이어 스태틱 메쉬 부착
	AttachWheelMeshToSocket();
}

void AAGVVehiclePawn::UpdateVehicleProperties(UAGVDataContainer* InAGVDataContainer)
{
	AGVDataContainer = InAGVDataContainer;
}

void AAGVVehiclePawn::AttachWheelMeshToSocket()
{
	if (const USkeletalMeshSocket* Socket = GetMesh()->GetSocketByName(FName("Socket_RR")))
	{
		WheelRR->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("VisWheel_BR"));
	}

	if (const USkeletalMeshSocket* Socket = GetMesh()->GetSocketByName(FName("Socket_FR")))
	{
		WheelFR->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("VisWheel_FR"));
	}

	if (const USkeletalMeshSocket* Socket = GetMesh()->GetSocketByName(FName("Socket_RL")))
	{
		WheelRL->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("VisWheel_BL"));
	}

	if (const USkeletalMeshSocket* Socket = GetMesh()->GetSocketByName(FName("Socket_FL")))
	{
		WheelFL->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("VisWheel_FL"));
	}
}

void AAGVVehiclePawn::UpdateVehiclePosition(const FVector& TargetLocation, float TargetYaw)
{
	FVector CurrentLocation = GetActorLocation();
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	
	// 현재 엔진의 토크 값 가져오기
	float CurrentEngineTorque = GetEngineTorque();

}

float AAGVVehiclePawn::GetEngineTorque()
{
	UChaosVehicleMovementComponent* MovementComponent = GetVehicleMovementComponent();
	// Chaos Vehicle의 엔진 컴포넌트에서 현재 토크 값 가져오기
	if (MovementComponent && MovementComponent->)
	{
		return ChaosVehicle->VehicleComponent->GetEngineTorque();
	}

	// 기본값 반환 (엔진 토크가 없을 경우)
	return 0.0f;
}

void AAGVVehiclePawn::SetAGVData(UAGVDataContainer* NewDataContainer)
{
	AGVDataContainer = NewDataContainer;
}


void AAGVVehiclePawn::MoveForward(float Value)
{
}

void AAGVVehiclePawn::MoveRight(float Value)
{
}

void AAGVVehiclePawn::ApplyHandbrake(bool bPressed)
{
}
