// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AGVDataTypes.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct FPosition
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	float X;

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	float Y;

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	float Z;

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	float Yaw;

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	float Pitch;

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	float Roll;

	FPosition():
		X(0.0f), Y(0.0f), Z(0.0f), Yaw(0.0f), Pitch(0.0f), Roll(0.0f) {};
};

USTRUCT(BlueprintType)
struct FAGVData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	FString RobotId;

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	FString Timestamp;

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	FPosition Position;

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	FString TargetObject;

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	FString Status;

	FAGVData() :
		RobotId(""), Timestamp(""), Position(), TargetObject(""), Status("") {};
};

USTRUCT(BlueprintType)
struct FFacilityInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	FString ID;

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	FVector Location;

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite, Category = "AMR")
	FString AssetPath;
};


UCLASS()
class HAE_API UAGVDataTypes : public UDataTable
{
	GENERATED_BODY()
};