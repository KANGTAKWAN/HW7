
#include "MyActor2.h"

AMyActor2::AMyActor2()
{
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(SceneRoot);
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
}

void AMyActor2::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector(100.0f, 100.0f, 100.0f));
	SetActorRotation(FRotator(0.0f, 45.0f, 0.0f));
	SetActorScale3D(FVector(2.0f));
	StartLocation = GetActorLocation();
	CurrentLocation = StartLocation;
	//MoveSpeed = FVector(0.0f, 0.0f, 50.0f);
	MaxRange = 300.0f;
}

void AMyActor2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentLocation += MoveSpeed * DeltaTime;
	SetActorLocation(CurrentLocation);
	UE_LOG(LogTemp, Display, TEXT("%s"),*CurrentLocation.ToString());
	UE_LOG(LogTemp, Display, TEXT("%s"), *MoveSpeed.ToString());

	if (FVector::Dist(CurrentLocation, StartLocation) > MaxRange)
	{
		MoveSpeed *= -1;
	}
}


