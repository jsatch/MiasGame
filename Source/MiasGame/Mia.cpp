// Fill out your copyright notice in the Description page of Project Settings.


#include "Mia.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MiasPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"

AMia::AMia()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
    PlayerCamera->SetupAttachment(SpringArm);
}

void AMia::BeginPlay() 
{
    Super::BeginPlay();

    MiaPlayerController = Cast<AMiasPlayerController>(GetController());

    FlipbookComponent = FindComponentByClass<UPaperFlipbookComponent>();

    if (MiaPlayerController)
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MiaPlayerController->GetLocalPlayer()))
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(InputMapping, 0);
        }
    }

}

void AMia::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        UE_LOG(LogTemp, Display, TEXT("Entra aca"));
        Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMia::Move);
        Input->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMia::StopMoving);
        Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMia::DoJump);
    }
    
}

void AMia::Move(const FInputActionValue& Value)
{
    const FVector2D MoveValue = Value.Get<FVector2D>();

    FVector DeltaLocation(0.f);
    FRotator Rotation(0.f);

    if (MoveValue.X < 0.f)
    {
        Rotation.Yaw = -180.f;
    }else
    {
        Rotation.Yaw = 0.f;
    }
    
    DeltaLocation.X = MoveValue.X * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLocation);
    //AddActorWorldOffset(DeltaLocation);
    FlipbookComponent->SetWorldRotation(Rotation);
    FlipbookComponent->SetFlipbook(FlipbookRun1);
}

void AMia::StopMoving(const FInputActionValue& Value)
{
    FlipbookComponent->SetFlipbook(FlipbookIdle);
}

void AMia::DoJump()
{
    Jump();

}
