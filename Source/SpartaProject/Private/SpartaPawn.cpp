#include "SpartaPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "PawnPlayerController.h"

ASpartaPawn::ASpartaPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	Body = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body"));
	SetRootComponent(Body);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 300.f;

	Body->SetSimulatePhysics(false);
	Body->SetCollisionProfileName(TEXT("OverlapAll"));
}

void ASpartaPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector StartLocation = GetActorLocation();
	StartLocation += FVector(0.0f, 0.0f, 50.0f); // 높이를 조금 올려줌.

	FVector EndLocationWithLocalForwardVector = StartLocation + (GetActorForwardVector() * 100.0f);
	FVector EndLocationWorldForwardVector = StartLocation + (FVector::ForwardVector * 100.0f);

	DrawDebugDirectionalArrow(GetWorld(), StartLocation, EndLocationWithLocalForwardVector, 50.0f, FColor::Red, false, 0.f, 0, 2.0f);
	DrawDebugDirectionalArrow(GetWorld(), StartLocation, EndLocationWorldForwardVector, 50.0f, FColor::Green, false, 0.f, 0, 2.0f);
}

void ASpartaPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (APawnPlayerController* PlayerController = Cast<APawnPlayerController>(GetController()))
		{
			if (PlayerController->MoveForwardAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveForwardAction, ETriggerEvent::Triggered, this, &ASpartaPawn::MoveForward);
			}

			if (PlayerController->MoveRightAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveRightAction, ETriggerEvent::Triggered, this, &ASpartaPawn::MoveRight);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ASpartaPawn::Look);
			}
		}
	}
}

void ASpartaPawn::MoveForward(const FInputActionValue& InValue)
{
	float ForwardInput = InValue.Get<float>();
	//UE_LOG(LogTemp, Log, TEXT("ForwardInput: %.1f"), ForwardInput);
	AddActorLocalOffset(FVector(ForwardInput, 0.f, 0.f));
	//AddActorWorldOffset(FVector(ForwardInput, 0.f, 0.f));
}

void ASpartaPawn::MoveRight(const FInputActionValue& InValue)
{
	float RightInput = InValue.Get<float>();
	//UE_LOG(LogTemp, Log, TEXT("RightInput: %.1f"), RightInput);
	AddActorLocalOffset(FVector(0.f, RightInput, 0.f));
	//AddActorWorldOffset(FVector(0.f, RightInput, 0.f));
}

void ASpartaPawn::Look(const FInputActionValue& InValue)
{
	float LookInput = InValue.Get<float>();
	//UE_LOG(LogTemp, Log, TEXT("LookInput: %.1f"), LookInput);
	AddActorLocalRotation(FRotator(0.f, LookInput, 0.f));
	//AddActorWorldRotation(FRotator(0.f, LookInput, 0.f));
}
