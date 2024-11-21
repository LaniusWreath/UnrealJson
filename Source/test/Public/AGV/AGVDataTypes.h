// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AGVDataTypes.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct FAGVData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	float x;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	float y;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	float theta;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	int32 l_enc;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	int32 r_enc;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	float odo_l;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	float odo_r;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	float yaw;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	float pitch;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	float roll;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	float orientation_x;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	float orientation_y;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	float orientation_z;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	float orientation_w;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	int32 gear_ratio;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	float wheel_separation;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AGV")
	float wheel_radius;

	FAGVData() 
		: x(0.0f), y(0.0f), theta(0.0f), l_enc(0), r_enc(0),
		odo_l(0.0f), odo_r(0.0f), yaw(0.0f), pitch(0.0f), roll(0.0f),
		orientation_x(0.0f), orientation_y(0.0f), orientation_z(0.0f),
		orientation_w(0.0f), gear_ratio(0), wheel_separation(0.0f),
		wheel_radius(0.0f) {}
};

UCLASS()
class TEST_API UAGVDataTypes : public UDataTable
{
	GENERATED_BODY()
};