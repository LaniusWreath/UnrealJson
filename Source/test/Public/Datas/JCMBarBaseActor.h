// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JCMBarBaseActor.generated.h"

class UTextRenderComponent;
class UTimelineComponent;
class UProceduralMeshComponent;

UCLASS()
class TEST_API AJCMBarBaseActor : public AActor
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

	int32 SpawnCount = 0;

	UPROPERTY()
	float ResultCustomStaticMeshSpawnedValue;

	// TimeLine Animation Binding Function
	UFUNCTION()
	void OnAnimationUpdate(float Value); // 타임라인에 바인딩될 함수는 UFUNCTION() 안붙이면 안됨

	void CreateCustomMeshRoutine(float BarHeight);
	void CreateCustomMeshRoutine(float BarHeight, int amount);

	void CreateSingleCustomMeshComponent(float BarHeight, float UnitMeshHeight, int32 SpawnAmount);

	void CreateAdditionalCustomMeshComponent(float BarHeight, float restHeight, float UnitMeshHeight);

	void InitializeCustomStaticMeshPhysics(UStaticMeshComponent* TargetStaticMesh,
		UStaticMeshComponent* TemplateComponent);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AJCMBarBaseActor();

	// On : Spawning Custom Mesh, Off: Spawning Default Bar Mesh
	UPROPERTY(EditAnywhere, Category = "Chart")
	bool EnableSpawnCustomMesh;

	UPROPERTY(VisibleAnywhere, Category = "Chart")
	UProceduralMeshComponent* ProcMeshComponent;

	// Specify Static Mesh to Generate, Don't Forget to Check off isProceduralMeshUsing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	UStaticMeshComponent* CustomStaticMeshComponent;

	// Bar Procedural Mesh Material
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!EnableSpawnCustomMesh"), Category = "Chart")
	UMaterialInstance* MeshMaterial;

	// Bar Generate Animation Curve
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!EnableSpawnCustomMesh"), Category = "Chart")
	UCurveFloat* AnimationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponentValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponentLabel;

	// Bar Mesh Width
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!EnableSpawnCustomMesh"), Category = "Chart")
	float Width_bar = 100.f;

	// Scaling Mesh Unit Size. 1 is Default (You Can also Adjust its Mesh Scale with MeshTransform x,y,z)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!EnableSpawnCustomMesh"), Category = "Chart")
	int UnitSize = 1;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "EnableSpawnCustomMesh"), Category = "Chart")
	bool SpawnPerUnitValue= false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "SpawnPerUnitValue"), Category = "Chart")
	float UnitValue=1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "EnableSpawnCustomMesh"), Category = "Chart")
	float CustomMeshSpawnWaitingTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart")
	float TextRenderComponentOffset_Value = 10;

	void ClearCustomMeshes();
	void ClearSpawnTimerHandle();

	UFUNCTION(BlueprintCallable, Category = "Chart")
	FVector GetStaticMeshBoxUnitSize(UStaticMesh* TargetStaticMesh) const;

	// Animation Control Function
	UFUNCTION()
	void PlayBarAnimation();

	// Create Procedural Mesh
	UFUNCTION(BlueprintCallable, Category = "Chart")
	void CreateProceduralBoxMesh(float BarHeight);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	void CreateMesh(float BarHeight, int Value);

	// Initialize Lbel Text Mesh
	UFUNCTION(BlueprintCallable, Category = " Chart")
	void InitializeTextMeshLabel(const FString& LabelName);

	// Initialize Value Text Mesh
	UFUNCTION(BlueprintCallable, Category = "Chart")
	void InitializeTextMeshValue(const float& FloatValue);

	void AdjustTextMeshValueOffset(const float& BarHeight);
	void AdjustTextMeshValueOffset(const int& amount);

	UFUNCTION(BlueprintCallable, Category = "Chart")
	float GetResultCustomMeshSpawnedValue() const
	{
		return ResultCustomStaticMeshSpawnedValue;
	}

	void BindTimelineAnimation();
};
