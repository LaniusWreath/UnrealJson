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

	int32 UnitSize = 1;

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
	void OnAnimationUpdate(float Value); // 타임라인에 바인딩될 함수는 UFUNCTION() 붙여야 함

	// Mesh Create Preference Routine
	void CreateCustomMeshRoutine(float BarHeight);
	void CreateCustomMeshRoutine(float BarHeight, int32 amount);
	void CreateCustomMeshRoutine();

	// Creating Mesh
	void CreateSingleCustomMeshComponent(const float BarHeight, const float UnitMeshHeight, int32 SpawnAmount, bool bUseInventory);
	// Creating Mesh Only One
	void CreateSingleCustomMeshComponent(const float UnitMeshHeight, const bool bUseInventory);


	void CreateAdditionalCustomMeshComponent(float BarHeight, float restHeight, float UnitMeshHeight);

	void InitializeCustomStaticMeshPhysics(UStaticMeshComponent* TargetStaticMesh,
		UStaticMeshComponent* TemplateComponent);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// On : Spawning custom mesh, Off: Spawning default bar mesh
	UPROPERTY(EditAnywhere, Category = "JCM")
	bool bEnableSpawnCustomMesh;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bEnableSpawnCustomMesh"), Category = "JCM")
	bool bUseStaticMeshInventory;

	// Add Static Mesh to Spawn, you have to match
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUseStaticMeshInventory"), Category = "JCM")
	TArray<UStaticMesh*> StaticMeshComponentInventory;

	UPROPERTY(VisibleAnywhere, Category = "JCM")
	UProceduralMeshComponent* ProcMeshComponent;

	// Specify static mesh to generate, Don't forget to check off isProceduralMeshUsing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JCM")
	UStaticMeshComponent* CustomStaticMeshTemplateComponent;

	// Bar procedural mesh material
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!EnableSpawnCustomMesh"), Category = "JCM")
	UMaterialInstance* MeshMaterial;

	// Bar generate animation curve
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!EnableSpawnCustomMesh"), Category = "JCM")
	UCurveFloat* AnimationCurve;

	// Visulalizing each JCM label data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JCM")
	UTextRenderComponent* TextRenderComponentValue;

	// Visualizing each JCM value data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JCM")
	UTextRenderComponent* TextRenderComponentLabel;

	// Basic bar mesh width
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!EnableSpawnCustomMesh"), Category = "JCM")
	float Width_bar = 100.f;

	// Spawning custom mesh with user-defined division unit size
	UPROPERTY(EditAnywhere, meta = (EditCondition = "EnableSpawnCustomMesh"), Category = "JCM")
	bool SpawnPerUnitValue= false;

	// Division unit size
	UPROPERTY(EditAnywhere, meta = (EditCondition = "SpawnPerUnitValue"), Category = "JCM")
	float UnitValue=10;

	// Custom mesh spawning delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "EnableSpawnCustomMesh"), Category = "JCM")
	float CustomMeshSpawnWaitingTime = 0.5f;

	// TextRender(Value) offset from JCM mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JCM")
	float TextRenderComponentOffset_Value = 30;

public:
	// Sets default values for this actor's properties
	AJCMBarBaseActor();

	void ClearCustomMeshes();
	void ClearSpawnTimerHandle();

	// Set StaticMeshComponent same as TemplateStaticMeshComponent
	void InitializeStaticMeshProperty(UStaticMeshComponent* TargetStaticMeshComponent, 
		const UStaticMeshComponent* TemplateMeshComponent);

	// Set StaticMesh in StaticMeshComponent with Mesh Inventory
	void InitializeStaticMeshPropertyFromInventory(UStaticMeshComponent* TargetStaticMeshComponent,
		const int32 InInventoryIndex);

	// Get StaticMesh from Inventory
	UStaticMesh* GetStaticMeshFromInventory(const int32 InInventoryIndex);

	// Get current custom mesh unit size
	UFUNCTION(BlueprintCallable, Category = "JCM")
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
	UFUNCTION(BlueprintCallable, Category = "JCM")
	int32 GetCustomMeshSpawnedAmount() const
	{
		return SpawnedCustomMeshAmount;
	}

	void BindTimelineAnimation();

	// Calculated custom mesh unit height
	UFUNCTION(BlueprintCallable, Category = "JCM")
	float GetCustomMeshUnitHeight();
};
