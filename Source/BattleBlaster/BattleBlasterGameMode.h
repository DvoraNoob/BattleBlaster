// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tank.h"
#include "GameFramework/GameModeBase.h"
#include "BattleBlasterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

public:
	ATank* Tank;

	int32 TowerCount;

	void ActorDied(AActor* DeadActor);

	bool IsVictory = false;

	UPROPERTY(EditAnywhere)
	float GameOverDelay = 3.f;

	UFUNCTION()
	void OnGameOverTimerTimeout();
};
