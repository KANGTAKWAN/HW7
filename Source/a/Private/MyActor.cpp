#include "MyActor.h"

DEFINE_LOG_CATEGORY(LogSparta);

AMyActor::AMyActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SceneRoot);
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(StaticMeshComp);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Resources/Props/SM_AssetPlatform.SM_AssetPlatform"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Resources/Materials/M_Metal_Gold.M_Metal_Gold"));
	if (MaterialAsset.Succeeded())
	{
		StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> AudioAsset(TEXT("/Game/Resources/Audio/Crate_Break.Crate_Break"));
	if (AudioAsset.Succeeded())
	{
		AudioComp->SetSound(AudioAsset.Object);
	}
	RotationSpeed = 90.0f; 
}
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector(200.0f, 300.0f, 100.0f));
	SetActorRotation(FRotator(0.0f, 45.0f, 0.0f));
	SetActorScale3D(FVector(2.0f));
}
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!FMath::IsNearlyZero(RotationSpeed))
	{
		AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
		UE_LOG(LogTemp, Display, TEXT("My Item appears!!"));
	}
}
void AMyActor::ResetActorPosition()
{
	SetActorLocation(FVector::ZeroVector);
}

float AMyActor::GetRotationSpeed() const
{
	return RotationSpeed;
}
///Script/Engine.StaticMesh'/Game/Resources/Props/SM_AssetPlatform.SM_AssetPlatform'