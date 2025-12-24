// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "BasePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis("MoveForward", this, &ATank::Move);
    PlayerInputComponent->BindAxis("Turn", this, &ATank::Turn);
    PlayerInputComponent->BindAxis("RotateTurret", this, &ATank::RotateTurret);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
    PlayerControllerRef = Cast<APlayerController>(GetController());
    GamepadInputControllerRef = Cast<APlayerController>(GetController());

}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FHitResult HitResult;
    if(PlayerControllerRef){
        PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

        // DrawDebugSphere(
        // GetWorld(),
        // HitResult.ImpactPoint,
        // 25.f,
        // 12,
        // FColor::Red,
        // true,
        // -1.f
        // );

        RotateTurret(HitResult.ImpactPoint);
    }
}


void ATank::Move(float Value){
    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLocation, true);

    // UE_LOG(LogTemp, Display, TEXT("Value %h"), DeltaLocation);
}

void ATank::Turn(float Value){
    FRotator DeltaRotation = FRotator::ZeroRotator;
    DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);
}

void ATank::RotateTurret(float Value){
    if (FMath::Abs(Value) > 0.05f)
    {
        float RotateAmount = Value * TurretRotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
        FRotator DeltaRotation = FRotator(0.f, RotateAmount, 0.f);
        if(TurretMesh)
        {
            TurretMesh->AddLocalRotation(DeltaRotation);
        }
    }
}