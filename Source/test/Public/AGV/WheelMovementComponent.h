// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WheelMovementComponent.generated.h"

class UAGVDataContainer;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEST_API UWheelMovementComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWheelMovementComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AGV")
	UAGVDataContainer* DataContainerRef;

	UFUNCTION(BlueprintCallable, Category = "AGV")
	void UpdateMovement(float DeltaTime);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
