// Fill out your copyright notice in the Description page of Project Settings.


#include "Datas/JCMDataContainer.h"

///////////////////////////////////////// 데이터 Get ////////////////////////////////////////////////
const EJCMChartTypes UJCMDataContainer::GetChartType() const
{
    return ChartType;
}
///////////////////////////////////////// 데이터 Set ///////////////////////////////////////////////

bool UJCMDataContainerXY::SetChartData(const FJCMChartDataXY& InputData)
{
    return false;
}

bool UJCMDataContainerXY::SetChartData(const FString& XName, const TArray<float>& XData, const FString& YName, const TArray<float>& YData)
{
    return false;
}

bool UJCMDataContainerXYZ::SetChartData(const FJCMChartDataXYZ& InputData)
{
    return false;
}

bool UJCMDataContainerXYZ::SetChartData(const TArray<FString>& LabelNames, const TArray<FVector>& VectorValues)
{
    return false;
}

UJCMDataContainerBar* UJCMDataContainerBar::SetChartData(const FJCMChartDataShape& InputData)
{
    ShapeChartData = InputData;
    if (ShapeChartData.ChartType == "")
    {
        UE_LOG(LogTemp, Warning, TEXT("UBarShapeComponent.cpp : %s.ChartType is Empty"), *this->GetName());
        return nullptr;
    }
    if (ShapeChartData.XName == "" || ShapeChartData.YName == "")
    {
        UE_LOG(LogTemp, Warning, TEXT("UBarShapeComponent.cpp : %s.Name is Empty"), *this->GetName());
        return nullptr;
    }
    if (ShapeChartData.Labels.Num() == 0 || ShapeChartData.Values.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBarShapeComponent.cpp : %s.Data is Empty"), *this->GetName());
        return nullptr;
    }
    
    return this;
}

UJCMDataContainerBar* UJCMDataContainerBar::SetChartData(const FString& ChartTitle, const FString& ChartTypeName, const FString& XName, const TArray<FString>& Labels, const FString& YName, const TArray<float>& Values)
{
    ShapeChartData.ChartTitle = ChartTitle;
    ShapeChartData.ChartType = ChartTypeName;
    ShapeChartData.XName = XName;
    ShapeChartData.Labels = Labels;
    ShapeChartData.YName = YName;
    ShapeChartData.Values = Values;

    if (ShapeChartData.ChartType == "")
    {
        UE_LOG(LogTemp, Warning, TEXT("UBarShapeComponent.cpp : %s.ChartType is Empty"), *this->GetName());
        return nullptr;
    }
    if (ShapeChartData.XName == "" || ShapeChartData.YName == "")
    {
        UE_LOG(LogTemp, Warning, TEXT("UBarShapeComponent.cpp : %s.Name is Empty"), *this->GetName());
        return nullptr;
    }
    if (ShapeChartData.Labels.Num() == 0 || ShapeChartData.Values.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBarShapeComponent.cpp : %s.Data is Empty"), *this->GetName());
        return nullptr;
    }

    return this;
}