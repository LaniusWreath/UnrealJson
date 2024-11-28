// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "AGVVehiclePawn.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API AAGVVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	
public:
	AAGVVehiclePawn();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void ApplyHandbrake(bool bPressed);

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraRig;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;
};
