// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "InputActionValue.h"
#include "AGVVehiclePawn.generated.h"

/**
 * 
 */
UCLASS()
class HAE_API AAGVVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	
public:
	AAGVVehiclePawn();
	
	// Attach Wheel Mesh to Skeletal Mesh Socket 
	void AttachWheelMeshToSocket();

// Data
	// Set AGVDataContainer
	UFUNCTION(BlueprintCallable, Category = "AGV")
	void SetAGVData(UAGVDataContainer* InDataContainer);	

// Movement
	UFUNCTION(BlueprintCallable, Category = "AGV")
	void UpdateVehiclePosition(const FVector& InLocation, float InYaw);

// Input
	// Initialize Player Input Component
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Move Forward
	void HandleThrottleInput(const FInputActionValue& Value);

	// Move Forward Release
	void HandleThrottleRelease(const FInputActionValue& Value);

	// Steering
	void HandleSteeringInput(const FInputActionValue& Value);

	// Sterring Release
	void HandleSteeringRelease(const FInputActionValue& Value);

	// Brake
	void HandleBrakeInput(const FInputActionValue& Value);
	void HandleBrakeRelease(const FInputActionValue& Value);

	// Zoom
	void SetZoomSize(const FInputActionValue& Value);
	void SetTargetArmLengthWithDeltaTime(const float DeltaTime);


protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
// Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UArrowComponent> DirectionArrow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

// Player Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> PlayerInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ThrottleAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SteeringAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> BrakeAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ZoomAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	int32 DesiredZoomSize = 700;

// Data
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAGVDataContainer> AGVDataContainer;

	// IsMoving
	bool bIsMoving = 0;

// Vehicle Wheels
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WheelFL;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WheelFR;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WheelRL;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WheelRR;

};
