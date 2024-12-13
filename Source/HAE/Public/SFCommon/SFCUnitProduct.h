// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFCUnitProduct.generated.h"

UCLASS()
class HAE_API ASFCUnitProduct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASFCUnitProduct();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SFC")
	TObjectPtr<UStaticMeshComponent> CoverMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SFC")
	TObjectPtr<UStaticMeshComponent> CenterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SFC")
	FRotator RotationSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "SFC")
	void RotateMesh(float InDeltaTime);


};
