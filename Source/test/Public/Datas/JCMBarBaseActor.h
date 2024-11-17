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

	int UnitSize = 1;

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
	int32 SpawnedCustomMeshAmount;

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

	// On : Spawning custom mesh, Off: Spawning default bar mesh
	UPROPERTY(EditAnywhere, Category = "Chart")
	bool EnableSpawnCustomMesh;

	UPROPERTY(VisibleAnywhere, Category = "Chart")
	UProceduralMeshComponent* ProcMeshComponent;

	// Specify static mesh to generate, Don't forget to check off isProceduralMeshUsing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	UStaticMeshComponent* CustomStaticMeshTemplateComponent;

	// Bar procedural mesh material
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!EnableSpawnCustomMesh"), Category = "Chart")
	UMaterialInstance* MeshMaterial;

	// Bar generate animation curve
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!EnableSpawnCustomMesh"), Category = "Chart")
	UCurveFloat* AnimationCurve;

	// Visulalizing each chart label data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponentValue;

	// Visualizing each chart value data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chart")
	UTextRenderComponent* TextRenderComponentLabel;

	// Basic bar mesh width
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!EnableSpawnCustomMesh"), Category = "Chart")
	float Width_bar = 100.f;

	// Spawning custom mesh with user-defined division unit size
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EnableSpawnCustomMesh"), Category = "Chart")
	bool SpawnPerUnitValue= false;

	// Division unit size
	UPROPERTY(EditAnywhere, meta = (EditCondition = "SpawnPerUnitValue"), Category = "Chart")
	float UnitValue=10;

	// Custom mesh spawning delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "EnableSpawnCustomMesh"), Category = "Chart")
	float CustomMeshSpawnWaitingTime = 0.5f;

	// TextRender(Value) offset from chart mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chart")
	float TextRenderComponentOffset_Value = 30;

	void ClearCustomMeshes();
	void ClearSpawnTimerHandle();

	// Get current custom mesh unit size
	UFUNCTION(BlueprintCallable, Category = "Chart")
	FVector GetStaticMeshBoxUnitSize(UStaticMesh* TargetStaticMesh) const;

	// Animation Control Function
	UFUNCTION()
	void PlayBarAnimation();

	// Create procedural mesh
	UFUNCTION()
	void CreateProceduralBoxMesh(float BarHeight);

	// Create custom mesh
	UFUNCTION()
	void CreateMesh(float BarHeight, int Value);

	// Initialize Lbel Text Mesh
	UFUNCTION()
	void InitializeTextMeshLabel(const FString& LabelName);

	// Initialize Value Text Mesh
	UFUNCTION()
	void InitializeTextMeshValue(const float& FloatValue);

	void AdjustTextMeshValueOffset(const float& BarHeight);
	void AdjustTextMeshValueOffset(const int32& amount);

	// Custom mesh spawned amount
	UFUNCTION(BlueprintCallable, Category = "Chart")
	int32 GetCustomMeshSpawnedAmount() const
	{
		return SpawnedCustomMeshAmount;
	}

	void BindTimelineAnimation();

	// Calculated custom mesh unit height
	UFUNCTION(BlueprintCallable, Category = "Chart")
	float GetCustomMeshUnitHeight();
};
