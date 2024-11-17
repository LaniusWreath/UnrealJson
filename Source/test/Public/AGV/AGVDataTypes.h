// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGVDataTypes.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct FAGVData
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	float x;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	float y;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	float theta;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	int32 l_enc;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	int32 r_enc;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	int32 odo_l;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	int32 odo_r;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	float yaw;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	float pitch;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	float roll;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	float orientation_x;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	float orientation_y;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	float orientation_z;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	float orientation_w;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	int32 gear_ratio;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	float wheel_separation;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Wheel Data")
	float wheel_radius;
};

class TEST_API AGVDataTypes
{
public:
	AGVDataTypes();
	~AGVDataTypes();
};
