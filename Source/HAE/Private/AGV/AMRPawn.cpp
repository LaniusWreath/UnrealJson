// Fill out your copyright notice in the Description page of Project Settings.


#include "AGV/AMRPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AAMRPawn::AAMRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VehicleSkeletalMesh"));
	RootComponent = SkeletalMesh;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true; // 카메라 독립 회전 허용

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	DirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction Arrow"));
	DirectionArrow->SetupAttachment(RootComponent);
}

// 이동 및 회전 함수
void AAMRPawn::MoveToTarget(const FVector& TargetLocation, const FRotator& TargetRotation)
{
	// 위치 보간
	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, GetWorld()->DeltaTimeSeconds, 5.0f);

	// 회전 보간
	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->DeltaTimeSeconds, 5.0f);

	// 위치와 회전 설정
	SetActorLocationAndRotation(NewLocation, NewRotation);
}

// Called when the game starts or when spawned
void AAMRPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAMRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAMRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

