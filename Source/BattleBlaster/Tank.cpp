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
		EIC->BindAction(AimTurretAction, ETriggerEvent::Canceled, this, &ATank::AimTurretAnalog);
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

void ATank::AimTurretAnalog(const FInputActionValue& Value)
{
	double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	FVector2D InputValue = Value.Get<FVector2D>();
	
	if (InputValue.Size() >= AnalogDeadZone.LeftStick)
	{
		bIsAnalog = true;
		
		float RotationAngle = FMath::RadiansToDegrees(FMath::Atan2(InputValue.X, -InputValue.Y));
		FRotator TurretCurRotation = TurretMesh->GetRelativeRotation();

		FRotator DeltaRotation = FRotator(0.f, RotationAngle, 0.f);
		
		FRotator TurretNewRotation = FMath::RInterpConstantTo(TurretCurRotation, DeltaRotation, DeltaTime, TurretAimSpeed);

		TurretMesh->SetRelativeRotation(TurretNewRotation);
	}
	else
	{
		bIsAnalog = false;
	}
}

void ATank::AimTurretMouse()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		float X, Y;
		PlayerController->GetMousePosition(X, Y);
		FVector2D CurrentMousePos(X, Y);
		
		bMouseMoved = CurrentMousePos != LastMousePos;
		
		if (bMouseMoved)
		{
			LastMousePos = CurrentMousePos;

			FHitResult HitResult;
			PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.f, 10, FColor::Red, false, 0.f, 0, 10);

			TurnTurret(HitResult.ImpactPoint);
		}

	}
}