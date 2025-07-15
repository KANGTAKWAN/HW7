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

	// 스켈레탈 메시 컴포넌트
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	SkeletalMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f)); // 캡슐 높이 보정
	SkeletalMeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // 정면 회전 보정

	// 스프링암 컴포넌트
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	// 카메라 컴포넌트
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Resources/Characters/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"));
	if (MeshAsset.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMesh(MeshAsset.Object);
	}
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));
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
	if (Controller && MovementVector != FVector2D::ZeroVector)
	{
		// 메쉬가 -90도 회전했기 때문에 이동 방향을 90도 보정
		const FRotator Rotation = FRotator(0, -90.f, 0); // 오른쪽을 앞쪽으로 바꿈

		// 현재 Forward, Right 벡터를 계산
		const FVector Forward = Rotation.RotateVector(GetActorForwardVector());
		const FVector Right = -Rotation.RotateVector(GetActorRightVector());

		// 부호 반전 (앞뒤 좌우 모두 반전)
		const FVector ReversedForward = -Forward;
		const FVector ReversedRight = -Right;

		AddMovementInput(ReversedForward, MovementVector.Y);
		AddMovementInput(ReversedRight, MovementVector.X);
	}
}
void AMyPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);     // 좌우 회전
		AddControllerPitchInput(LookAxisVector.Y);   // 위아래 회전
	}
}
