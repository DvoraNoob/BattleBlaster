// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.h"
#include "Tower.generated.h"



/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ATower : public ABasePawn
{
	GENERATED_BODY()

public:
	ATower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	ATank* Tank;

	UPROPERTY()
	FQuat StartQuat;

	UPROPERTY()
	bool bTankIsInRange = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float FireRate = 2.f;
	
	UFUNCTION()
	void CheckCanFire();
	
};
