// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(
        FireRateTimerHandle,
        this,
        &ATower::CheckFireCondition,
        FireRateTime,
        true
    );
}

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Find the direction to the Tank (the player)
    if(InFireRange()){
        RotateTurret(Tank->GetActorLocation());
    }
}

void ATower::CheckFireCondition(){
    if(InFireRange()){
        Fire();
    }
}

bool ATower::InFireRange(){
    if(Tank){
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
        if(Distance <= FireRate){
            return true;
        }
    }
    return false;
}