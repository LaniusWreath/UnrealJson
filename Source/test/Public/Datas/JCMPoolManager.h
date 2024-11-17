// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JCMPoolManager.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UJCMPoolManager : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TArray<UStaticMeshComponent*> Pool; // 풀링된 컴포넌트 리스트

	UPROPERTY()
	AActor* OwnerActor;

	int32 PoolSize;

public:
	void Initialize(AActor* Owner, int32 Size);

};
