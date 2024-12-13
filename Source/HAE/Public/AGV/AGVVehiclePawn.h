// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
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

	UFUNCTION(BlueprintCallable, Category = "AGV")
	void SetAGVData(UAGVDataContainer* NewDataContainer);

	void UpdateVehicleProperties(UAGVDataContainer* AGVDataContainer);


	UFUNCTION(BlueprintCallable, Category = "AGV")
	void CallUpdateWheelRadiusRoutine(float InWheelRadius, float OriginRadius);
	void UpdateWheelRadius(float InWheelRadius, float OriginRadius);
	void UpdateWheelMeshScale(float InWheelRadius, float OriginRadius);

	void UpdateWheelPhysics(float InWheelRadius, float OriginRadius);

	UFUNCTION(BlueprintCallable, Category = "AGV")


	void AttachWheelMeshToSocket();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void ApplyHandbrake(bool bPressed);


// Components
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAGVDataContainer> AGVDataContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WheelFL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WheelFR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WheelRL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WheelRR;

};
