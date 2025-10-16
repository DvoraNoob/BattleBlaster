// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISense_Sight.h"
#include "AIController_Tower.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API AAIController_Tower : public AAIController
{
	GENERATED_BODY()

public:
	AAIController_Tower();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Sight* SightSConfig;

	UFUNCTION()
	void OnTargetDetected(AActor* SensedActor, FAIStimulus Stimulus);
};
