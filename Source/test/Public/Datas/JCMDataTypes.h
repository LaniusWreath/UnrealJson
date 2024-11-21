// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JCMDataTypes.generated.h"

// Struct for Shape Chart Data
USTRUCT(BlueprintType)
struct FJCMChartDataShape
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ChartTitle;

	// Chart Types : "bar", "line", "pie" .. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ChartType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString XName;

	// Label for X Axis
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Labels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString YName;
	
	// Value for Y Axis or Quotiont for PieChart
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Values;
};

USTRUCT(BlueprintType)
struct FJCMChartDataXY
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FString ChartTitle;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FString XName;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<float> XData;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FString YName;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<float> YData;
};

USTRUCT(BlueprintType)
struct FJCMChartPoint
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FVector Coordinate;
};

USTRUCT(BlueprintType)
struct FJCMChartDataXYZ
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FString ChartTitle;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<FJCMChartPoint> Points;
};

// ¿­°ÅÇü
UENUM(BlueprintType)
enum EJCMChartTypes : uint8
{
	NONE = 0 UMETA(DisplayName = "NONE"),
	BAR = 1 UMETA(DisplayName = "BAR"),
	LINE = 2 UMETA(DisplayName = "LINE"),
	PIE = 3 UMETA(DisplayName = "PIE"),
	XY = 4 UMETA(DisplayName = "XY"),
	XYZ = 5 UMETA(DisplayName = "XYZ"),
	FREE = 6 UMETA(DisplayName = "FREE"),
};

class TEST_API JCMDataTypes
{
public:
	JCMDataTypes();

	UPROPERTY(VisibleAnywhere)
	static TMap<FString, EJCMChartTypes> JCMMapChartTypes ;

	~JCMDataTypes();
};


