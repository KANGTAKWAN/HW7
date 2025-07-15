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

	// ĸ�� ������Ʈ
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->InitCapsuleSize(42.f, 96.f); // (������, ����)
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComp->SetCollisionObjectType(ECC_Pawn); // ������ ����
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore); // �⺻ ��� ����
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // ���� Static�� ����
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // �ٸ� Pawn�� ��ħ

	// ���̷�Ż �޽� ������Ʈ
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	SkeletalMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f)); // ĸ�� ���� ����
	SkeletalMeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // ���� ȸ�� ����

	// �������� ������Ʈ
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	// ī�޶� ������Ʈ
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
		// �޽��� -90�� ȸ���߱� ������ �̵� ������ 90�� ����
		const FRotator Rotation = FRotator(0, -90.f, 0); // �������� �������� �ٲ�

		// ���� Forward, Right ���͸� ���
		const FVector Forward = Rotation.RotateVector(GetActorForwardVector());
		const FVector Right = -Rotation.RotateVector(GetActorRightVector());

		// ��ȣ ���� (�յ� �¿� ��� ����)
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
		AddControllerYawInput(LookAxisVector.X);     // �¿� ȸ��
		AddControllerPitchInput(LookAxisVector.Y);   // ���Ʒ� ȸ��
	}
}
