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

	if (UsingMouse())
	{
		AimTurretMouse();
	}
}

void ATank::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::MoveInput);
		EIC->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::TurnInput);
		
		EIC->BindAction(AimTurretAction, ETriggerEvent::Triggered, this, &ATank::AimTurretAnalog);
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
		float RotationAngle = FMath::RadiansToDegrees(FMath::Atan2(InputValue.X, -InputValue.Y));
		
		FQuat CurQuat = TurretMesh->GetRelativeRotation().Quaternion();
		FQuat TargetQuat = FRotator(0.f, RotationAngle, 0.f).Quaternion();

		FQuat TurretNewRotation = FMath::QInterpTo(CurQuat, TargetQuat, DeltaTime, TurretTurnSpeed);
		
		TurretMesh->SetRelativeRotation(TurretNewRotation);
	}
}

void ATank::AimTurretMouse()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (!UsingMouse())
			return;
		
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.f, 10, FColor::Red, false, 0.f, 0, 10);

		TurnTurret(HitResult.ImpactPoint);

	}
}

bool ATank::MouseMoved() const
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		float X, Y;

		PlayerController->GetMousePosition(X, Y);
		FVector2D CurrentMousePos(X, Y);
		
		return CurrentMousePos != LastMousePos;
	}

	return false;
}

bool ATank::UsingMouse() const
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
		return false;
	
	if (UInputDeviceSubsystem* InputDeviceSubsystem = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>())
	{
		const FPlatformUserId UserId = PlayerController->GetPlatformUserId();
		FHardwareDeviceIdentifier DeviceIdentifier = InputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(UserId);

		return  (DeviceIdentifier.PrimaryDeviceType == EHardwareDevicePrimaryType::KeyboardAndMouse);
	}

	return false;
}