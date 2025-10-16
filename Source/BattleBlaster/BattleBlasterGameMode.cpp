// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"

#include "Tower.h"
#include "Kismet/GameplayStatics.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Variable to store each Tower in the map/level
	TArray<AActor*> Towers;
	// Get all Tower class actors in the level and stores each one in the Towers array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	// Get the numbers of elements in the Towers array and set it as TowerCount
	TowerCount = Towers.Num();

	// Get player pawn 0 in the level
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		// Try to Convert the player Pawn into an ATank
		Tank = Cast<ATank>(PlayerPawn);
		// If cast failure, show a debug message.
		if (!Tank)
		{
			UE_LOG(LogTemp, Error, TEXT("GameMode: Tank actor not found!"));
		}
	}

	// Do a loop on each Actor in the Towers Array
	for (AActor* TowerActor : Towers)
	{
		// Try to convert the AActor into an ATower and verify if this is valid (success convert),
		// also verify if Tank is valid.
		ATower* Tower = Cast<ATower>(TowerActor);
		if (Tower && Tank)
		{
			// Set the Tank variable from Tower with the Tank found here in GameMode
			Tower->Tank = Tank;
		}
	}
}
