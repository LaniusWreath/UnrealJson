// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EduGameInstance.generated.h"

/**
 * 
 */

UCLASS()
class TEST_API UEduGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

};
