// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "MyActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSparta, Log, All);

UCLASS()
class A_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyActor|Components")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyActor|Components")
	UStaticMeshComponent* StaticMeshComp;
	UAudioComponent* AudioComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MyActor|Properties")
	float RotationSpeed;

	UFUNCTION(BlueprintCallable, Category = "Item|Actions")
	void ResetActorPosition();

	UFUNCTION(BlueprintPure, Category = "Item|Properties")
	float GetRotationSpeed() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Item|Event")
	void OnItemPickedUp();
};
