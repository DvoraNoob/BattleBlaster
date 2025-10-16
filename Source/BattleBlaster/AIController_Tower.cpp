// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Tower.h"

#include "Tower.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"

AAIController_Tower::AAIController_Tower()
{
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
	SetPerceptionComponent(*AIPerceptionComp );

	SightSConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SightSense");
	SightSConfig->SightRadius = 1000.f;
	SightSConfig->LoseSightRadius = 1200.f;
	SightSConfig->PeripheralVisionAngleDegrees = 180.f;
	SightSConfig->NearClippingRadius = 25.f;
	
	AIPerceptionComp->ConfigureSense(*SightSConfig);
	AIPerceptionComp->SetDominantSense(SightSConfig->GetSenseImplementation());
}

void AAIController_Tower::BeginPlay()
{
	Super::BeginPlay();
}

void AAIController_Tower::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AIPerceptionComp)
	{
		AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_Tower::OnTargetDetected);
	}
}

void AAIController_Tower::OnTargetDetected(AActor* SensedActor, FAIStimulus Stimulus)
{
	ATower* Tower = Cast<ATower>(GetPawn());
	ATank* Tank = Cast<ATank>(SensedActor);

	if (Tank && Tower)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			Tower->bTankIsInRange = true;
		}
		else
		{
			Tower->bTankIsInRange = false;
		}
	}
}
