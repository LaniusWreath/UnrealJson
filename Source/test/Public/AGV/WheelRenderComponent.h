// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "WheelRenderComponent.generated.h"

/**
 * 
 */

class UStaticMeshComponent;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEST_API UWheelRenderComponent : public UPrimitiveComponent
{
	GENERATED_BODY()
	
public:
    UWheelRenderComponent();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AGV")
    UStaticMeshComponent* ChassisMeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV")
    FVector CollisionExtent;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AGV")
    bool isGround;

    UFUNCTION(BlueprintCallable, Category = "AGV")
    void SetWheelMesh(UStaticMesh* Mesh);

protected:
    virtual void BeginPlay() override;
};
