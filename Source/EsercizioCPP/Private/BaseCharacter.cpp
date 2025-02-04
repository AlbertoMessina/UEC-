// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->bUsePawnControlRotation = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	CameraArm->TargetArmLength = 220.0f;
	
	CameraArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(CameraArm);
	
	bUseControllerRotationPitch = true;

	//SET BASE SPEED
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::MoveForward(float InputValue) {
	FVector ForwardDirction = GetActorForwardVector();
	AddMovementInput(ForwardDirction,InputValue, false);
}

void ABaseCharacter::MoveRight(float InputValue) {
	FVector RightDirection = GetActorRightVector();
	AddMovementInput(RightDirection, InputValue, false);
}

void ABaseCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	bIsSprinting = true;
	SprintAlpha = 0.0f;
	GetWorld()->GetTimerManager().ClearTimer(SprintTimer);
	
}

void ABaseCharacter::StopSprint()
{
	if (SprintAlpha > 1.0f)
	{
		SprintAlpha = 1.0f;
	}

	float InitSpeed = GetCharacterMovement()->MaxWalkSpeed;
	float NewSpeed = FMath::Lerp(InitSpeed, WalkSpeed, SprintAlpha);
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	UE_LOG(LogTemp, Log, TEXT("STOP SPRINTING") );
	
	if (NewSpeed > WalkSpeed && SprintAlpha < 1.0f)
	{
		bIsSprinting = false;
		SprintAlpha += GetWorld()->GetDeltaSeconds() * SprintSpeed;
		UE_LOG(LogTemp, Log, TEXT("{Alpha: %f}"), SprintAlpha);
		GetWorld()->GetTimerManager().SetTimer(SprintTimer, this, &ABaseCharacter::StopSprint, 0.2f, false);
		 
	}

}

void ABaseCharacter::Crouch()
{
	GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
	UE_LOG(LogTemp, Log, TEXT("CROUCHING") );
}

void ABaseCharacter::StopCrouch()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	UE_LOG(LogTemp, Log, TEXT("STOP CROUCHING") );
}

void ABaseCharacter::LookUp(float InputValue)
{

	FRotator CRelativeRot = CameraArm->GetRelativeRotation();
	
	FRotator FinalRotation = CRelativeRot.Add(InputValue ,0.0f, 0.0f );
	FinalRotation.Pitch = FMath::Clamp(FinalRotation.Pitch, -45, 45);
	
	CameraArm->SetRelativeRotation(FinalRotation);
	//AddControllerPitchInput(InputValue);
	//UE_LOG(LogTemp, Log, TEXT("{LookUp: %f}"), InputValue);
}

void ABaseCharacter::Turn(float InputValue)
{
	AddControllerYawInput(InputValue);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FHitResult Hit;

	FVector TraceStart = Camera->GetComponentLocation() + Camera->GetForwardVector() * CameraArm->TargetArmLength;
	FVector TraceEnd = TraceStart + (Camera->GetForwardVector() * Distance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart,TraceEnd,ECollisionChannel::ECC_GameTraceChannel1, QueryParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
	//UE_LOG(LogTemp, Log, TEXT("Tracing line: %s to %s"), *TraceStart.ToCompactString(), *TraceEnd.ToCompactString());

	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		UE_LOG(LogTemp, Log, TEXT("Trace hit actor : %s"), *Hit.GetActor()->GetName());
	}
	//else {
	//	UE_LOG(LogTemp, Log, TEXT("NO ACOTR HIT	"));
	//}
	//// To run the query, you need a pointer to the current level, which you can get from an Actor with GetWorld()
	//// UWorld()->LineTraceSingleByChannel runs a line trace and returns the first actor hit over the provided collision channel.
	//GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);

	//// You can use DrawDebug helpers and the log to help visualize and debug your trace queries.
	//

	//// If the trace hit something, bBlockingHit will be true,
	//// and its fields will be filled with detailed info about what was hit
	//if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *Hit.GetActor()->GetName());
	//}
	//else {
	//	UE_LOG(LogTemp, Log, TEXT("No Actors were hit"));
	//}

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABaseCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABaseCharacter::StopSprint);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABaseCharacter::Crouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABaseCharacter::StopCrouch);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);

	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ABaseCharacter::Turn);

	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::LookUp);

}