// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor2.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSparta, Log, All);

UCLASS()
class A_API AMyActor2 : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyActor2();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyActor|Components")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyActor|Components")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyActor|Components")
	FVector StartLocation;
	FVector CurrentLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyActor|Components")
	FVector MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyActor|Components")
	float MaxRange;
};
