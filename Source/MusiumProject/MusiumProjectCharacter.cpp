// Copyright Epic Games, Inc. All Rights Reserved.

#include "MusiumProjectCharacter.h"
#include "MusiumProjectProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


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

void AMusiumProjectCharacter::EnableLineTrace_Implementation(bool bEnable)
{
	bCanLineTrace = bEnable;
	if (bEnable == false)
	{
		UE_LOG(LogTemp, Display, TEXT("Linetrace Disable"));
		CurrentTarget = nullptr; // 라인트레이스 비활성화 시 타겟 초기화
	}
}

void AMusiumProjectCharacter::ShowInteractionUI_Implementation(const FInteractionData& Data)
{
	// TODO: UMG 위젯을 띄워서 Data.Title/Data.Description 표시
}

void AMusiumProjectCharacter::OnInteract()
{
	if (CurrentTarget)
	{
		// 1) UObject* 로 캐스트
		AActor* TargetActor = Cast<AActor>(CurrentTarget);
		if (TargetActor && TargetActor->Implements<UIInteractiveTarget>())
		{
			// 2) Execute_Interact 에 두 인자 모두 넘기기
			IIInteractiveTarget::Execute_Interact(TargetActor, this);
		}
	}
}


//리팩토링 필요
void AMusiumProjectCharacter::performLineTrace()
{
	// 1) 뷰포인트 & 레이 끝점
	FVector Start;
	FRotator Rot;
	Controller->GetPlayerViewPoint(Start, Rot);
	FVector End = Start + Rot.Vector() * TraceDistance;

	// 2) 라인트레이스
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit, Start, End, ECC_Visibility, Params
	);

	// 3) NewTarget 결정 (오직 인터페이스 구현 대상만)
	IIInteractiveTarget* NewTarget = nullptr;
	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor
			&& HitActor->GetClass()->ImplementsInterface(UIInteractiveTarget::StaticClass()))
		{
			NewTarget = Cast<IIInteractiveTarget>(HitActor);
		}
	}

	// 4) 타깃 변경 시에만 HoverEnd/Start 호출
	if ((NewTarget != CurrentTarget) || (NewTarget == nullptr))
	{
		// 이전 타깃이 있으면 HoverEnd
		if (CurrentTarget)
		{
			AActor* PrevActor = Cast<AActor>(CurrentTarget);
			CurrentTarget->Execute_OnHoverEnd(PrevActor);
			UE_LOG(LogTemp, Display, TEXT("hoveringend 실행됨"));
		}
		// 새 타깃이 있으면 HoverStart
		if (NewTarget)
		{
			AActor* NewActor = Cast<AActor>(NewTarget);
			NewTarget->Execute_OnHoverStart(NewActor);
		}
		// 상태 갱신
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
