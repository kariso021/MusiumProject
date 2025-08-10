// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActor/InteractiveActor.h"
#include "Components/WidgetComponent.h"
#include "InteractionInterface/IInteractionReceiver.h"
#include "GameFramework/Character.h"

// Sets default values
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	// 여기서 지정할 이유는 없으니까. false 로 해서 Tick 없앰
	PrimaryActorTick.bCanEverTick = false;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRootComponent;


	// 기본매쉬
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	//충돌 범위용 박스 컴포넌트
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetBoxExtent(FVector(100.f));                   // 기본 크기
	BoxComponent->SetCollisionProfileName(TEXT("Trigger"));      // 채널은 Trigger로 설정

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveActor::HandleOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AInteractiveActor::HandleOverlapEnd);

	// InteractionWidget 컴포넌트
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetupAttachment(RootComponent);
	InteractionWidget->SetHiddenInGame(false);
	InteractionWidget->SetVisibility(true);

	// InteractionComponent 컴포넌트
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));




}

void AInteractiveActor::OnHoverStart_Implementation()
{

	float Now = GetWorld()->GetTimeSeconds();
	//  TODO : Tick 으로 계속 호출 방지를 위해 hover 시작 시점, 그리고 끝날때를 분리해야할듯
	if (!isHovering)
	{
		UE_LOG(LogTemp, Display, TEXT("Hovering Start"));
		OnHoverStart.Broadcast(ActorID, Now);
		InteractionComponent->HoverStart(); // InteractionComponent에서 호버 시작 처리 Glow 효과 등
		isHovering = true; // 호버 상태를 true로 변경
	}

}

void AInteractiveActor::OnHoverEnd_Implementation()
{
	float Now = GetWorld()->GetTimeSeconds();
	if (isHovering)
	{
		UE_LOG(LogTemp, Display, TEXT("Hovering End"));
		isHovering = false; // 호버 상태를 false로 변경
		InteractionComponent->HoverEnd(); // InteractionComponent에서 호버 끝 처리 Glow 효과 등
	}
	OnHoverEnd.Broadcast(ActorID, Now);
}

void AInteractiveActor::Interact_Implementation(AActor* Interactor)
{
	OnInteract.Broadcast(ActorID);
	//여기서 Broadcast를 한 이후, 캐릭터에서 제어
}

FInteractionData AInteractiveActor::GetInteractionData_Implementation()
{
	return InteractionData;
}

// Called when the game starts or when spawned
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AInteractiveActor::HandleOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UIInteractionReceiver>())
	{
		IIInteractionReceiver::Execute_EnableLineTrace(OtherActor, true);
	}
}

void AInteractiveActor::HandleOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UIInteractionReceiver>())
	{
		// 호버가 끝났을 때, LineTrace를 비활성화
		IIInteractionReceiver::Execute_EnableLineTrace(OtherActor, false); // 이름을 잘못지었군
		// 호버 종료 이벤트 호출

		// OnHoverEnd 이벤트 호출
		float Now = GetWorld()->GetTimeSeconds();
		if (isHovering)
		{
			UE_LOG(LogTemp, Display, TEXT("Hovering End"));
			isHovering = false; // 호버 상태를 false로 변경
			InteractionComponent->HoverEnd(); // InteractionComponent에서 호버 끝 처리 Glow 효과 등
		}
		OnHoverEnd.Broadcast(ActorID, Now);
	}
}
