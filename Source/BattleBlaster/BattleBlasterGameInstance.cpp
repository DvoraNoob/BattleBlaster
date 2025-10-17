// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UBattleBlasterGameInstance::LoadNextLevel()
{
	if (CurrentLevelIndex < MaxlevelIndex)
	{
		ChangeLevel(CurrentLevelIndex + 1);
	}
	else
	{
		// Open Main Menu or Modal  
	}
}

void UBattleBlasterGameInstance::RestartLevel()
{
	ChangeLevel(CurrentLevelIndex);
}

void UBattleBlasterGameInstance::RestartGame()
{
	ChangeLevel(1);
}

void UBattleBlasterGameInstance::ChangeLevel(int32 LevelIndex)
{
	if (LevelIndex >= 1 && LevelIndex <= MaxlevelIndex)
	{
		CurrentLevelIndex = LevelIndex;

		FString LevelName = FString::Printf(TEXT("Level_%d"), CurrentLevelIndex);
		UGameplayStatics::OpenLevel(this, *LevelName);
	}
}
