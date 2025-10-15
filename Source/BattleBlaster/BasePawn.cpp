// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

}

void ABasePawn::TurnTurret(const FVector& LookTarget) const
{
	const FVector TurretDirection = LookTarget - TurretMesh->GetComponentLocation();
	
	FQuat const CurQuat = TurretMesh->GetComponentRotation().Quaternion();
	FQuat const TargetQuat = FRotator(0.f, TurretDirection.Rotation().Yaw, 0.f).Quaternion();
	FQuat const TurretRotation = FMath::QInterpTo(CurQuat, TargetQuat, GetWorld()->GetDeltaSeconds(), TurretTurnSpeed);
	
	TurretMesh->SetWorldRotation(TurretRotation);
}


