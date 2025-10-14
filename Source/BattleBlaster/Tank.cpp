// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "EnhancedInputComponent.h"
#include "INodeAndChannelMappings.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CapsuleComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArm);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
				UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATank::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::MoveInput);
		EIC->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::TurnInput);
	}
}

void ATank::MoveInput(const FInputActionValue& Value)
{
	double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	
	//float InputValue = Value.Get<float>();
	float InputValue = Value.Get<float>();

	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = InputValue * MoveSpeed * DeltaTime;
	AddActorLocalOffset(DeltaLocation, true, nullptr, ETeleportType::None);
}

void ATank::TurnInput(const FInputActionValue& Value)
{
	double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	float InputValue = Value.Get<float>();

	FRotator DeltaRotation = FRotator(0.f, InputValue, 0.f) * TurnSpeed * DeltaTime;
	AddActorLocalRotation(DeltaRotation, true, nullptr, ETeleportType::None);
}
