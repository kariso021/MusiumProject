// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveActor/InteractiveActor.h"
#include "Components/WidgetComponent.h"
#include "InteractionInterface/IInteractionReceiver.h"
#include "GameFramework/Character.h"

// Sets default values
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	// ���⼭ ������ ������ �����ϱ�. false �� �ؼ� Tick ����
	PrimaryActorTick.bCanEverTick = false;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRootComponent;


	// �⺻�Ž�
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	//�浹 ������ �ڽ� ������Ʈ
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetBoxExtent(FVector(100.f));                   // �⺻ ũ��
	BoxComponent->SetCollisionProfileName(TEXT("Trigger"));      // ä���� Trigger�� ����

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveActor::HandleOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AInteractiveActor::HandleOverlapEnd);

	// InteractionWidget ������Ʈ
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetupAttachment(RootComponent);
	InteractionWidget->SetHiddenInGame(false);
	InteractionWidget->SetVisibility(true);

	// InteractionComponent ������Ʈ
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));




}

void AInteractiveActor::OnHoverStart_Implementation()
{

	float Now = GetWorld()->GetTimeSeconds();
	//  TODO : Tick ���� ��� ȣ�� ������ ���� hover ���� ����, �׸��� �������� �и��ؾ��ҵ�
	if (!isHovering)
	{
		UE_LOG(LogTemp, Display, TEXT("Hovering Start"));
		OnHoverStart.Broadcast(ActorID, Now);
		InteractionComponent->HoverStart(); // InteractionComponent���� ȣ�� ���� ó�� Glow ȿ�� ��
		isHovering = true; // ȣ�� ���¸� true�� ����
	}

}

void AInteractiveActor::OnHoverEnd_Implementation()
{
	float Now = GetWorld()->GetTimeSeconds();
	if (isHovering)
	{
		UE_LOG(LogTemp, Display, TEXT("Hovering End"));
		isHovering = false; // ȣ�� ���¸� false�� ����
		InteractionComponent->HoverEnd(); // InteractionComponent���� ȣ�� �� ó�� Glow ȿ�� ��
	}
	OnHoverEnd.Broadcast(ActorID, Now);
}

void AInteractiveActor::Interact_Implementation(AActor* Interactor)
{
	OnInteract.Broadcast(ActorID);
	//���⼭ Broadcast�� �� ����, ĳ���Ϳ��� ����
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
		// ȣ���� ������ ��, LineTrace�� ��Ȱ��ȭ
		IIInteractionReceiver::Execute_EnableLineTrace(OtherActor, false); // �̸��� �߸�������
		// ȣ�� ���� �̺�Ʈ ȣ��

		// OnHoverEnd �̺�Ʈ ȣ��
		float Now = GetWorld()->GetTimeSeconds();
		if (isHovering)
		{
			UE_LOG(LogTemp, Display, TEXT("Hovering End"));
			isHovering = false; // ȣ�� ���¸� false�� ����
			InteractionComponent->HoverEnd(); // InteractionComponent���� ȣ�� �� ó�� Glow ȿ�� ��
		}
		OnHoverEnd.Broadcast(ActorID, Now);
	}
}
