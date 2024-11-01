// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BarBaseActor.generated.h"

class UTextRenderComponent;
class UTimelineComponent;
class UProceduralMeshComponent;

UCLASS()
class TEST_API ABarBaseActor : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY()
	UTimelineComponent* BarAnimationTimeline;

	UPROPERTY()
	USceneComponent* DefaultSceneRootComponent;

	// Custom Static Mesh Component Offset Layer
	UPROPERTY()
	USceneComponent* CustomActorSceneComponent;

	// Custome Static Mesh Spawn Timer
	FTimerHandle SpawnTimerHandle;

	int32 SpawnCount;

	UPROPERTY()
	int32 ResultCustomStaticMeshSpawnedAmount;

	UPROPERTY()
	float ResultCustomStaticMeshSpawnedValue;

	// TimeLine Animation Binding Function
	UFUNCTION()
	void OnAnimationUpdate(float Value);

	UFUNCTION()
	void CreateCustomMeshRoutine(float BarHeight);

	UFUNCTION()
	void CreateSingleCustomMeshComponent(float BarHeight, float UnitMeshHeight, int32 SpawnAmount);

	UFUNCTION()
	void CreateAdditionalCustomMeshComponent(float BarHeight, float restHeight, float UnitMeshHeight);

	UFUNCTION()
	void InitializeCustomStaticMeshPhysics(UStaticMeshComponent* TargetStaticMesh,
		UStaticMeshComponent* TemplateComponent);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ABarBaseActor();

	UPROPERTY(EditDefaultsOnly, Category = "Chart")
	UProceduralMeshComponent* ProcMeshComponent;

	// Specify Static Mesh to Generate, Don't For Get to Check off isProceduralMeshUsing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	UStaticMeshComponent* CustomStaticMeshComponent;

	// Bar Procedural Mesh Material
	UPROPERTY(EditAnywhere, Category = "Chart")
	UMaterialInstance* MeshMaterial;

	// Bar Generate Animation Curve
	UPROPERTY(EditAnywhere, Category = "Chart")
	UCurveFloat* AnimationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponentValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponentLabel;

	// Bar Mesh Width
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart")
	float Width_bar = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart")
	int UnitSize = 1;

	// Bar Text Unit Size : Label
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart")
	float ValueTextPadding = 15.f;

	// On : Spawning Custom Mesh, Off: Spawning Default Bar Mesh
	UPROPERTY(EditAnywhere, Category = "Chart")
	bool EnableSpawnCustomMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart")
	float CustomMeshSpawnWaitingTime = 0.5f;

	UFUNCTION(BlueprintCallable, Category = "Chart")
	FVector GetStaticMeshBoxUnitSize(UStaticMesh* TargetStaticMesh) const;

	// Animation Control Function
	UFUNCTION()
	void PlayBarAnimation();

	// Create Procedural Mesh
	UFUNCTION(BlueprintCallable, Category = "Chart")
	void CreateProceduralBoxMesh(float BarHeight);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void CreateMesh(float BarHeight);

	// Initialize Lbel Text Mesh
	UFUNCTION(BlueprintCallable, Category = " Chart")
	void InitializeTextMeshLabel(const FString& LabelName);

	// Initialize Value Text Mesh
	UFUNCTION(BlueprintCallable, Category = "Chart")
	void InitializeTextMeshValue(const float& FloatValue, const float& BarHeight);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	int32 GetResultCustomMeshSpawnedAmount() const
	{
		return ResultCustomStaticMeshSpawnedAmount;
	}

	UFUNCTION(BlueprintCallable, Category = "Chart")
	float GetResultCustomMeshSpawnedValue() const
	{
		return ResultCustomStaticMeshSpawnedValue;
	}

};
