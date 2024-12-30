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

protected:
	virtual void BeginPlay() override;

private:
// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UArrowComponent> DirectionArrow;

// Player Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> PlayerInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ThrottleAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SteeringAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> BrakeAction;

// Data
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAGVDataContainer> AGVDataContainer;

	// IsMoving
	bool bIsMoving = 0;

	

// Vehicle Wheels
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WheelFL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WheelFR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WheelRL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WheelRR;

};
