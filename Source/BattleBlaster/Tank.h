// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"

#include "EnhancedInputSubsystems.h"

#include "GameFramework/SpringArmComponent.h"
#include "Tank.generated.h"

// Forward declarations
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;

USTRUCT(BlueprintType)
struct FDeadZones
{
	GENERATED_BODY()

	float LeftStick = 0.2f;
	float RightStick = 0.2f;
};

UCLASS()
class BATTLEBLASTER_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AimTurretAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 300.f;
	
	UPROPERTY(EditAnywhere)
	float TurnSpeed = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	FDeadZones AnalogDeadZone;

	bool bMouseMoved = false;
	FVector2D LastMousePos = FVector2D::ZeroVector;

	bool bMouseIsActive = false;	
	bool bShowMouseCursor = false;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UFUNCTION()
	void MoveInput(const FInputActionValue& Value);

	UFUNCTION()
	void TurnInput(const FInputActionValue& Value);

	UFUNCTION()
	void FireInput(const FInputActionValue& Value);

	UFUNCTION()
	void AimTurretAnalog(const FInputActionValue& Value);

	UFUNCTION()
	void AimTurretMouse();

	UFUNCTION()
	void MouseMoved (const APlayerController* PlayerController);
};
