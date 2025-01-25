// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AMRPawn.generated.h"

UCLASS()
class HAE_API AAMRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAMRPawn();

	UFUNCTION(BlueprintCallable, Category = "AGV")
	void MoveToTarget(const FVector& TargetLocation, const FRotator& TargetRotation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UArrowComponent> DirectionArrow;

// Datas
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AGV", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAGVDataContainer> DataContainer;



};
