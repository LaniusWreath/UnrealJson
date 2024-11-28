// Fill out your copyright notice in the Description page of Project Settings.

#include "AGV/AGVVehiclePawn.h"
#include "AGVVehiclePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AAGVVehiclePawn::AAGVVehiclePawn()
{
    CameraRig = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    CameraRig->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    Camera->SetupAttachment(RootComponent);
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
