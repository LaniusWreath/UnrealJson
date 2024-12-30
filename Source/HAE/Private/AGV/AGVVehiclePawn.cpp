// Fill out your copyright notice in the Description page of Project Settings.

#include "AGV/AGVVehiclePawn.h"
#include "AGV/AGVLog.h"
#include "AGV/AGVDataContainer.h"
#include "Components/StaticMeshComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "ChaosVehicles/Public/ChaosVehicleWheel.h"
#include "ChaosVehicleMovementComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"


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

	DirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction Arrow"));
	DirectionArrow->SetupAttachment(RootComponent);
}

void AAGVVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	// 섀시 스켈레탈 메쉬 소켓에 타이어 스태틱 메쉬 부착
	AttachWheelMeshToSocket();

	// Enhanced Input Subsystem 가져오기
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem && PlayerInputMappingContext)
		{
			// Input Mapping Context 추가
			Subsystem->AddMappingContext(PlayerInputMappingContext, 0);
		}
	}
}

// 휠 메시 소켓에 부착
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

// 인풋
void AAGVVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		// Throttle 함수 바인딩
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AAGVVehiclePawn::HandleThrottleInput);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AAGVVehiclePawn::HandleThrottleRelease);

		// Steering 함수 바인딩
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AAGVVehiclePawn::HandleSteeringInput);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &AAGVVehiclePawn::HandleSteeringRelease);

		// 브레이크 함수 바인딩
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AAGVVehiclePawn::HandleBrakeInput);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &AAGVVehiclePawn::HandleBrakeRelease);
	}
}

// 직진 함수
void AAGVVehiclePawn::HandleThrottleInput(const FInputActionValue& Value)
{
	float ThrottleValue = Value.Get<float>();

	if (GetVehicleMovementComponent())
	{
		if (ThrottleValue >= 0)
		{
			GetVehicleMovementComponent()->SetTargetGear(1, true);
			GetVehicleMovementComponent()->SetThrottleInput(ThrottleValue);
		}
		else
		{
			GetVehicleMovementComponent()->SetTargetGear(-1, true);
			GetVehicleMovementComponent()->SetThrottleInput(FMath::Abs(ThrottleValue));
		}
	}
}

// 스로틀 해제
void AAGVVehiclePawn::HandleThrottleRelease(const FInputActionValue& Value)
{
	if (GetVehicleMovementComponent())
	{
		// 입력 해제 시 스로틀 해제
		GetVehicleMovementComponent()->SetThrottleInput(0.0f); 
	}
}

// 핸들 조향
void AAGVVehiclePawn::HandleSteeringInput(const FInputActionValue& Value)
{
	float SteeringValue = Value.Get<float>();
	if (GetVehicleMovementComponent())
	{
		GetVehicleMovementComponent()->SetSteeringInput(SteeringValue);
	}
}

// 핸들 조향 해제
void AAGVVehiclePawn::HandleSteeringRelease(const FInputActionValue& Value)
{
	if (GetVehicleMovementComponent())
	{
		GetVehicleMovementComponent()->SetSteeringInput(0.0f); // 입력 해제 시 조향 정지
	}
}

// 브레이크 작동
void AAGVVehiclePawn::HandleBrakeInput(const FInputActionValue& Value)
{
	float BrakeValue = Value.Get<float>();
	if(GetVehicleMovementComponent())
	{
		GetVehicleMovementComponent()->SetBrakeInput(BrakeValue);
	}
}

// 브레이크 해제
void AAGVVehiclePawn::HandleBrakeRelease(const FInputActionValue& Value)
{
	if (GetVehicleMovementComponent())
	{
		GetVehicleMovementComponent()->SetBrakeInput(0);
	}
}

// 데이터 연동
void AAGVVehiclePawn::SetAGVData(UAGVDataContainer* InDataContainer)
{
	AGVDataContainer = InDataContainer;
}

// 차 포지션 및 회전 변경
void AAGVVehiclePawn::UpdateVehiclePosition(const FVector& InLocation, float InYaw)
{
	if (!GetVehicleMovementComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleMovementComponent not found!"));
		return;
	}

	// 현재 차량 위치와 방향
	FVector CurrentLocation = GetActorLocation();

	FRotator CurrentRotation = GetActorRotation();

	// 이동 방향 계산
	FVector Direction = (InLocation - CurrentLocation).GetSafeNormal();
	UE_LOG(AGVlog, Log, TEXT("Direction : %f, %f, %f"), Direction.X, Direction.Y, Direction.Z);

	// 차량의 핸들 방향 설정
	float SteeringAngle = FMath::Atan2(Direction.Y, Direction.X) - FMath::DegreesToRadians(CurrentRotation.Yaw);
	UE_LOG(AGVlog, Log, TEXT("SteeringAngle : %f"), SteeringAngle);

	GetVehicleMovementComponent()->SetSteeringInput(FMath::Clamp(SteeringAngle, -1.0f, 1.0f));

	//// 차량의 가속 설정
	//if (Distance > 100.0f) // 100cm 이상 거리만 가속
	//{
	//	GetVehicleMovementComponent()->SetThrottleInput(FMath::Clamp(Distance / 1000.0f, 0.0f, 1.0f));
	//}
	//else
	//{
	//	GetVehicleMovementComponent()->SetThrottleInput(0.0f); // 거리 가까울 경우 가속 중지
	//	GetVehicleMovementComponent()->SetBrakeInput(1.0f); // 브레이크 적용
	//}
}
