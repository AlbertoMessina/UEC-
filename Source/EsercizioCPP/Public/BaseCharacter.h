// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


UCLASS()
class ESERCIZIOCPP_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Camera")
	class UCameraComponent* Camera;

	class USpringArmComponent* CameraArm;

	FTimerHandle SprintTimer;

	float SprintAlpha = 0.0;

	bool bIsSprinting = false;

	// CAPIRECOME MODIFICARE LA CATEGORIZAZIONE DELL UPROPERTY
	UPROPERTY(EditAnywhere, Category = "Movement|Walk")
	float WalkSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Movement|Sprint")
	float SprintStopSpeed = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Movement|Sprint")
	float SprintSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Movement|CrouchW")
	float CrouchSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = "InteractionDistance")
	float Distance = 1000.0f;

	void StartSprint();

	void StopSprint();

	void Crouch();
	
	void StopCrouch();

	void MoveForward(float InputValue);

	void MoveRight(float InputValue);

	void LookUp(float InputValue);
	
	void Turn(float InputValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
