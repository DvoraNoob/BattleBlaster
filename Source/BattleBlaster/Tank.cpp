// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "EnhancedInputComponent.h"
#include "INodeAndChannelMappings.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Perception/AISense_Sight.h"

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

	StimuliSource = FindComponentByClass<UAIPerceptionStimuliSourceComponent>();

	PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
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

	SetPlayerInputEnabled(false);
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTurretMouse();
}

void ATank::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::MoveInput);
		EIC->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::TurnInput);
		
		EIC->BindAction(AimTurretAction, ETriggerEvent::Triggered, this, &ATank::AimTurretAnalog);

		EIC->BindAction(FireAction, ETriggerEvent::Started, this, &ATank::FireInput);
	}
}

void ATank::MoveInput(const FInputActionValue& Value)
{
	double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	
	float InputValue = Value.Get<float>();

	if (abs(InputValue) >= AnalogDeadZone.LeftStick)
	{
		FVector DeltaLocation = FVector::ZeroVector;
		DeltaLocation.X = InputValue * MoveSpeed * DeltaTime;
		AddActorLocalOffset(DeltaLocation, true, nullptr, ETeleportType::None);
	}
}

void ATank::TurnInput(const FInputActionValue& Value)
{
	double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	float InputValue = Value.Get<float>();

	if (abs(InputValue) >= AnalogDeadZone.LeftStick)
	{
		FRotator DeltaRotation = FRotator(0.f, InputValue, 0.f) * TurnSpeed * DeltaTime;
		AddActorLocalRotation(DeltaRotation, true, nullptr, ETeleportType::None);
	}
}

void ATank::FireInput(const FInputActionValue& Value)
{
	bool InputValue = Value.Get<bool>();

	if (InputValue)
	{
		FireTurret();
	}
}

void ATank::AimTurretAnalog(const FInputActionValue& Value)
{
	double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	FVector2D InputValue = Value.Get<FVector2D>();
	
	if (InputValue.Size() >= AnalogDeadZone.RightStick)
	{
		bMouseIsActive = false;
		
		float RotationAngle = FMath::RadiansToDegrees(FMath::Atan2(InputValue.X, -InputValue.Y));
		
		FQuat CurQuat = TurretMesh->GetRelativeRotation().Quaternion();
		FQuat TargetQuat = FRotator(0.f, RotationAngle, 0.f).Quaternion();

		FQuat TurretNewRotation = FMath::QInterpTo(CurQuat, TargetQuat, DeltaTime, TurretTurnSpeed);
		
		TurretMesh->SetRelativeRotation(TurretNewRotation);
	}
}

void ATank::AimTurretMouse()
{
	MouseMoved();

	if (PlayerController)
	{	
		if (bMouseIsActive)
		{
			if (!bMouseCursorVisible)
			{
				PlayerController->SetShowMouseCursor(true);
				bMouseCursorVisible = true;
			}

			FHitResult HitResult;
			PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, HitResult);
			
			TurnTurret(HitResult.ImpactPoint);
		}
		else
		{
			if (bMouseCursorVisible)
			{
				PlayerController->SetShowMouseCursor(false);
				bMouseCursorVisible = false;
			}
		}
	}
}

void ATank::MouseMoved()
{
	float X, Y;

	PlayerController->GetMousePosition(X, Y);
	FVector2D CurrentMousePos(X, Y);

	bMouseMoved = CurrentMousePos != LastMousePos;
		
	if (!bMouseIsActive && bMouseMoved)
	{
		LastMousePos = CurrentMousePos;
		bMouseIsActive = true;
			
	}
}

void ATank::SetPlayerInputEnabled(bool bEnable)
{
	if (bEnable) {
		EnableInput(PlayerController);
		// Enable Tick of actor
		SetActorTickEnabled(true);
	}
	else {
		DisableInput(PlayerController);
		PlayerController->SetShowMouseCursor(false);
		// Disable Tick of actor
		SetActorTickEnabled(false);
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	// Hide actor
	SetActorHiddenInGame(true);
	// Disable collision
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Check if StimulusSource is valid and unregister
	if (StimuliSource)
	{
		StimuliSource->UnregisterFromPerceptionSystem();
	}
	
	SetPlayerInputEnabled(false);

}
