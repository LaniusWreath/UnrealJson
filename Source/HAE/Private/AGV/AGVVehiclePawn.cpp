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
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"


AAGVVehiclePawn::AAGVVehiclePawn()
{
	WheelFL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel FL"));
	WheelFL->SetupAttachment(RootComponent);

	WheelFR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel FR"));
	WheelFR->SetupAttachment(RootComponent);

	WheelRL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel RL"));
	WheelRL->SetupAttachment(RootComponent);

	WheelRR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel RR"));
	WheelRR->SetupAttachment(RootComponent);

	DirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction Arrow"));
	DirectionArrow->SetupAttachment(RootComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = DesiredZoomSize;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AAGVVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	// ���� ���̷�Ż �޽� ���Ͽ� Ÿ�̾� ����ƽ �޽� ����
	AttachWheelMeshToSocket();

	// Enhanced Input Subsystem ��������
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem && PlayerInputMappingContext)
		{
			// Input Mapping Context �߰�
			Subsystem->AddMappingContext(PlayerInputMappingContext, 0);
		}
	}
}

void AAGVVehiclePawn::Tick(float DeltaTime)
{
	SetTargetArmLengthWithDeltaTime(DeltaTime);
}

// �� �޽� ���Ͽ� ����
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

// ��ǲ
void AAGVVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		// Throttle �Լ� ���ε�
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AAGVVehiclePawn::HandleThrottleInput);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AAGVVehiclePawn::HandleThrottleRelease);

		// Steering �Լ� ���ε�
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AAGVVehiclePawn::HandleSteeringInput);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &AAGVVehiclePawn::HandleSteeringRelease);

		// �극��ũ �Լ� ���ε�
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AAGVVehiclePawn::HandleBrakeInput);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &AAGVVehiclePawn::HandleBrakeRelease);

		// �� �Լ� ���ε�
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AAGVVehiclePawn::SetZoomSize);
	}
}

// ���� �Լ�
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

// ����Ʋ ����
void AAGVVehiclePawn::HandleThrottleRelease(const FInputActionValue& Value)
{
	if (GetVehicleMovementComponent())
	{
		// �Է� ���� �� ����Ʋ ����
		GetVehicleMovementComponent()->SetThrottleInput(0.0f); 
	}
}

// �ڵ� ����
void AAGVVehiclePawn::HandleSteeringInput(const FInputActionValue& Value)
{
	float SteeringValue = Value.Get<float>();
	if (GetVehicleMovementComponent())
	{
		GetVehicleMovementComponent()->SetSteeringInput(SteeringValue);
	}
}

// �ڵ� ���� ����
void AAGVVehiclePawn::HandleSteeringRelease(const FInputActionValue& Value)
{
	if (GetVehicleMovementComponent())
	{
		GetVehicleMovementComponent()->SetSteeringInput(0.0f); // �Է� ���� �� ���� ����
	}
}

// �극��ũ �۵�
void AAGVVehiclePawn::HandleBrakeInput(const FInputActionValue& Value)
{
	float BrakeValue = Value.Get<float>();
	if(GetVehicleMovementComponent())
	{
		GetVehicleMovementComponent()->SetBrakeInput(BrakeValue);
	}
}

// �극��ũ ����
void AAGVVehiclePawn::HandleBrakeRelease(const FInputActionValue& Value)
{
	if (GetVehicleMovementComponent())
	{
		GetVehicleMovementComponent()->SetBrakeInput(0);
	}
}

// ī�޶� �� ���� ����
void AAGVVehiclePawn::SetZoomSize(const FInputActionValue& Value)
{
	float InWheelValue = Value.Get<float>();
	if (GetVehicleMovementComponent())
	{
		float TempZoomSize = InWheelValue * 70 + DesiredZoomSize;
		DesiredZoomSize = FMath::Clamp(TempZoomSize, 100, 3000);
	}
}

// ī�޶� �� ���� ���� �ӵ� ���� ƽ ���� �Լ�
void AAGVVehiclePawn::SetTargetArmLengthWithDeltaTime(const float DeltaTime)
{
	if (!SpringArmComponent)
	{
		UE_LOG(AGVlog, Warning, TEXT("SpringArmComponent is invalid"));
		return;
	}
	float CurrentTargetArmLength = SpringArmComponent->TargetArmLength;
	SpringArmComponent->TargetArmLength = FMath::FInterpTo(CurrentTargetArmLength, DesiredZoomSize, DeltaTime, 2);
}

// ������ ����
void AAGVVehiclePawn::SetAGVData(UAGVDataContainer* InDataContainer)
{
	AGVDataContainer = InDataContainer;
}

// �� ������ �� ȸ�� ����
void AAGVVehiclePawn::UpdateVehiclePosition(const FVector& InLocation, float InYaw)
{
	if (!GetVehicleMovementComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleMovementComponent not found!"));
		return;
	}

	// ���� ���� ��ġ�� ����
	FVector CurrentLocation = GetActorLocation();

	FRotator CurrentRotation = GetActorRotation();

	// �̵� ���� ���
	FVector Direction = (InLocation - CurrentLocation).GetSafeNormal();
	UE_LOG(AGVlog, Log, TEXT("Direction : %f, %f, %f"), Direction.X, Direction.Y, Direction.Z);

	// ������ �ڵ� ���� ����
	float SteeringAngle = FMath::Atan2(Direction.Y, Direction.X) - FMath::DegreesToRadians(CurrentRotation.Yaw);
	UE_LOG(AGVlog, Log, TEXT("SteeringAngle : %f"), SteeringAngle);

	GetVehicleMovementComponent()->SetSteeringInput(FMath::Clamp(SteeringAngle, -1.0f, 1.0f));
	//GetVehicleMovementComponent()->SetThrottleInput(FMath::Clamp(Distance / 1000.0f, 0.0f, 1.0f));

	//// ������ ���� ����
	//if (Distance > 100.0f) // 100cm �̻� �Ÿ��� ����
	//{
	//	GetVehicleMovementComponent()->SetThrottleInput(FMath::Clamp(Distance / 1000.0f, 0.0f, 1.0f));
	//}
	//else
	//{
	//	GetVehicleMovementComponent()->SetThrottleInput(0.0f); // �Ÿ� ����� ��� ���� ����
	//	GetVehicleMovementComponent()->SetBrakeInput(1.0f); // �극��ũ ����
	//}
}
