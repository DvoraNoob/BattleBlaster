// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

#include "Projectile.h"
#include "Engine/StaticMeshSocket.h"

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

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(TurretMesh);

}

void ABasePawn::TurnTurret(const FVector& LookTarget) const
{
	const FVector TurretDirection = LookTarget - TurretMesh->GetComponentLocation();

	FQuat const TargetQuat = FRotator(0.f, TurretDirection.Rotation().Yaw, 0.f).Quaternion();
	
	FQuat const CurQuat = TurretMesh->GetComponentRotation().Quaternion();
	FQuat const TurretRotation = FMath::QInterpTo(CurQuat, TargetQuat, GetWorld()->GetDeltaSeconds(), TurretTurnSpeed);
	
	TurretMesh->SetWorldRotation(TurretRotation);

}

void ABasePawn::FireTurret()
{
	class UStaticMeshSocket const* SpawnSocket = TurretMesh->GetSocketByName("SpawnSocket");

	FTransform SpawnTransform;
	SpawnSocket->GetSocketTransform(SpawnTransform, TurretMesh);

	FVector SpawnLocation = SpawnTransform.GetLocation();
	FRotator SpawnRotation = SpawnTransform.Rotator();

	GetWorld()->SpawnActor<AProjectile>(ProjectileBP, SpawnLocation, SpawnRotation);
}


