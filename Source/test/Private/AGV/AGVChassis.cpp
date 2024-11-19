// Fill out your copyright notice in the Description page of Project Settings.


#include "AGV/AGVChassis.h"

// Sets default values
AAGVChassis::AAGVChassis()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAGVChassis::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAGVChassis::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

