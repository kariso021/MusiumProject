// Copyright Epic Games, Inc. All Rights Reserved.

#include "MusiumProjectCharacter.h"
#include "MusiumProjectProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"


//////////////////////////////////////////////////////////////////////////
// AMusiumProjectCharacter

AMusiumProjectCharacter::AMusiumProjectCharacter()
{
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));


	//Tick 제한
	PrimaryActorTick.TickInterval = 0.3f;

}

void AMusiumProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void AMusiumProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMusiumProjectCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMusiumProjectCharacter::Look);
		

		//Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMusiumProjectCharacter::OnInteract);
	}
}

void AMusiumProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCanLineTrace) return;

	// Perform line trace to detect interactive targets
	performLineTrace();
}


void AMusiumProjectCharacter::ShowInteractionUI_Implementation(const FInteractionData& Data)
{
	// TODO: UMG 위젯을 띄워서 Data.Title/Data.Description 표시
	// 그냥 playercontroller 에서 해서 생략.
}

void AMusiumProjectCharacter::EnteredInteractionZone_Implementation()
{
	OverlappingInteractionZones++;
	//uelog 로 InteractionZones 재고싶은데
	UE_LOG(LogTemp, Warning, TEXT("Entered Zone. Overlapping Zones Count: %d"), OverlappingInteractionZones);

	if (OverlappingInteractionZones > 0)
	{
		SetLineTraceEnabled(true);
	}
}

void AMusiumProjectCharacter::LeftInteractionZone_Implementation()
{
	OverlappingInteractionZones--;
	UE_LOG(LogTemp, Warning, TEXT("Left Zone. Overlapping Zones Count: %d"), OverlappingInteractionZones);
	if (OverlappingInteractionZones < 0)
	{
		OverlappingInteractionZones = 0;
	}

	if (OverlappingInteractionZones == 0)
	{
		SetLineTraceEnabled(false);
	}
}

void AMusiumProjectCharacter::OnInteract()
{
	if (CurrentTarget->Implements<UIInteractiveTarget>())
	{
		IIInteractiveTarget::Execute_Interact(CurrentTarget, this);
	}
}


void AMusiumProjectCharacter::performLineTrace()
{
	FVector Start;
	FRotator Rot;
	if (!Controller) return;

	Controller->GetPlayerViewPoint(Start, Rot);
	const FVector End = Start + Rot.Vector() * TraceDistance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	// [수정] 이제 일반 Visibility 채널로 라인트레이스를 쏩니다.
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	AActor* HitActor = HitResult.GetActor();
	AActor* NewTarget = nullptr;

	// 맞은 액터가 인터페이스를 가지고 있는지 확인
	if (HitActor && HitActor->Implements<UIInteractiveTarget>())
	{
		NewTarget = HitActor;
	}


	// 타겟이 변경되었는지 확인
	if (NewTarget != CurrentTarget)
	{
		// 이전 타겟이 있었다면 HoverEnd 호출
		if (CurrentTarget)
		{
			IIInteractiveTarget::Execute_OnHoverEnd(CurrentTarget);
		}

		// 새로운 타겟이 있다면 HoverStart 호출
		if (NewTarget)
		{
			IIInteractiveTarget::Execute_OnHoverStart(NewTarget);
		}

		// 현재 타겟 갱신
		CurrentTarget = NewTarget;
	}
}




void AMusiumProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMusiumProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
