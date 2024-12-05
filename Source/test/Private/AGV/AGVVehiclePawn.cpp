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

void AAGVVehiclePawn::CallUpdateWheelRadiusRoutine(float InWheelRadius, float OriginRadius)
{
	UpdateWheelRadius(InWheelRadius, OriginRadius);
	UpdateWheelMeshScale(InWheelRadius, OriginRadius);
}

// 카오스 비히클 휠 물리적 반지름 변경
void AAGVVehiclePawn::UpdateWheelRadius(float InWheelRadius, float OriginRadius)
{
	UChaosWheeledVehicleMovementComponent* VehicleMovement = 
		Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());
	
	if (!VehicleMovement)
	{
		UE_LOG(AGVlog, Error, TEXT("WheeledVehicleMovement Cast Failed"));
		return;
	}

	for (int32 WheelIndex = 0; WheelIndex < VehicleMovement->WheelSetups.Num(); WheelIndex++)
	{
		// 휠 반지름 변경 API 함수 호출
		VehicleMovement->SetWheelRadius(WheelIndex, InWheelRadius);

		/*float RadiusDifference = InWheelRadius - OriginRadius;
		VehicleMovement->WheelSetups[WheelIndex].AdditionalOffset.Z += RadiusDifference;*/

		float RadiusDifference = InWheelRadius - OriginRadius;
		VehicleMovement->WheelSetups[WheelIndex].AdditionalOffset.Z += RadiusDifference;
	}
	VehicleMovement->ResetVehicle();
}

// 휠 메쉬 스케일 업데이트
void AAGVVehiclePawn::UpdateWheelMeshScale(float InWheelRadius, float OriginRadius)
{
	if (!WheelFL || !WheelFR || !WheelRL || !WheelRR)
	{
		UE_LOG(AGVlog, Error, TEXT("Wheel meshes are invalid"));
		return;
	}

	TArray<UStaticMeshComponent*> WheelMeshes = { WheelFL, WheelFR, WheelRL, WheelRR };

	UChaosWheeledVehicleMovementComponent* VehicleMovement =
		Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent());
	if (!VehicleMovement)
	{
		UE_LOG(AGVlog, Error, TEXT("WheeledVehicleMovement Cast Failed"));
		return;
	}

	float Scaler = InWheelRadius / OriginRadius;

	for (int32 WheelIndex = 0; WheelIndex < VehicleMovement->WheelSetups.Num(); WheelIndex++)
	{
		if (UStaticMeshComponent* CurWheelMesh = WheelMeshes[WheelIndex])
		{
			FVector NewScale = CurWheelMesh->GetRelativeScale3D();
			NewScale *= Scaler;
			CurWheelMesh->SetRelativeScale3D(NewScale);

			//FVector NewLocation = CurWheelMesh->GetRelativeLocation();
			//float RadiusDifference = InWheelRadius - OriginRadius;
			//NewLocation.Z += RadiusDifference; // Z축 위치 보정
			//CurWheelMesh->SetRelativeLocation(NewLocation);

			// 디버깅
			//DrawDebugSphere(GetWorld(), NewLocation + FVector(0, 0, NewLocation.Z), InWheelRadius, 12, FColor::Red);
			FVector NewLocation = CurWheelMesh->GetRelativeLocation();
			float RadiusDifference = InWheelRadius - OriginRadius;
			NewLocation.Z += RadiusDifference; // Z축 위치 보정
			CurWheelMesh->SetRelativeLocation(NewLocation);

			// 디버깅
			DrawDebugSphere(GetWorld(), NewLocation + FVector(0, 0, NewLocation.Z), InWheelRadius, 12, FColor::Red);
		}
		else
		{
			UE_LOG(AGVlog, Error, TEXT("WheelMesh is invalid"));
			return;
		}
	}
}

void AAGVVehiclePawn::UpdateWheelPhysics(float InWheelRadius, float OriginRadius)
{
	if (!GetMesh() || !GetMesh()->GetPhysicsAsset()) return;

	UPhysicsAsset* PhysicsAsset = GetMesh()->GetPhysicsAsset();


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

void AAGVVehiclePawn::SetAGVData(UAGVDataContainer* NewDataContainer)
{
	AGVDataContainer = NewDataContainer;
}

void AAGVVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
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
