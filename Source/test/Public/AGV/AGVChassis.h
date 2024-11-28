// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AGVChassis.generated.h"

class UWheelMovementComponent;
class UWheelRenderComponent;
class UAGVDataContainer;

UCLASS()
class TEST_API AAGVChassis : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAGVChassis();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AGV")
	UAGVDataContainer* WheelDataContainer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	UWheelMovementComponent* WheelMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	UWheelRenderComponent* WheelRenderComponent;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraRig;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
