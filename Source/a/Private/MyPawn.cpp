#include "MyPawn.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캡슐 컴포넌트
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->InitCapsuleSize(42.f, 96.f); // (반지름, 높이)
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComp->SetCollisionObjectType(ECC_Pawn); // 폰으로 설정
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore); // 기본 모두 무시
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // 월드 Static엔 막힘
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 다른 Pawn은 겹침
	CapsuleComp->SetSimulatePhysics(false);

	// 스켈레탈 메시 컴포넌트
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	SkeletalMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f)); // 캡슐 높이 보정
	SkeletalMeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // 정면 회전 보정

	// 스프링암 컴포넌트
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = false;

	// 카메라 컴포넌트
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Resources/Characters/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"));
	if (MeshAsset.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMesh(MeshAsset.Object);
	}
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(GetController()))
		{
			if (MyPC->MoveAction)
			{
				EnhancedInput->BindAction(MyPC->MoveAction, ETriggerEvent::Triggered, this, &AMyPawn::Move);
			}

			if (MyPC->LookAction)
			{
				EnhancedInput->BindAction(MyPC->LookAction, ETriggerEvent::Triggered, this, &AMyPawn::Look);
			}
		}
	}
}
void AMyPawn::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (!MovementVector.IsNearlyZero())
	{
		// 이동 속도 (cm/s)
		const float MoveSpeed = 300.f;

		// DeltaTime 얻기
		float DeltaTime = GetWorld()->GetDeltaSeconds();

		// 이동 벡터를 로컬 좌표계로 변환 (X: 앞뒤, Y: 좌우)
		FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.f);

		// 실제 이동량
		FVector DeltaLocation = MoveDirection * MoveSpeed *DeltaTime;

		// 충돌 고려하며 위치 변경
		AddActorLocalOffset(DeltaLocation, false);

	}
}
void AMyPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	if (!LookVector.IsNearlyZero())
	{
		const float RotationSpeed = 90.f;
		float DeltaTime = GetWorld()->GetDeltaSeconds();

		// 좌우 회전 (Yaw)
		FRotator YawRotation(0.f, LookVector.X * RotationSpeed * DeltaTime, 0.f);
		AddActorLocalRotation(YawRotation);

		// 상하 회전 (Pitch)
		if (SpringArmComp)
		{
			FRotator SpringArmRotation = SpringArmComp->GetRelativeRotation();
			SpringArmRotation.Pitch = FMath::Clamp(SpringArmRotation.Pitch - LookVector.Y * RotationSpeed * DeltaTime, -80.f, 80.f);
			SpringArmComp->SetRelativeRotation(SpringArmRotation);
		}
	}
}