// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

ATower::ATower() {}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	StartQuat = TurretMesh->GetComponentRotation().Quaternion();

	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATower::CheckCanFire, 1.f, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTankIsInRange)
	{
		TurnTurret(Tank->GetActorLocation());
	}
	else
	{
		FQuat CurQuat = TurretMesh->GetComponentRotation().Quaternion();
		if (CurQuat != StartQuat)
		{
			FQuat TargetQuat = StartQuat;
			FQuat TurretRotation = FMath::QInterpTo(CurQuat, TargetQuat, DeltaTime, TurretTurnSpeed);
			
			TurretMesh->SetWorldRotation(TurretRotation);
		}
	}
}

void ATower::CheckCanFire()
{
	if (Tank && bTankIsInRange)
	{
		FireTurret();
	}
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();

	Destroy();
}
