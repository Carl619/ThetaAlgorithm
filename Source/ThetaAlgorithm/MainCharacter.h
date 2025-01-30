// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainCharacter.generated.h"

typedef class AMapManager;
typedef class UAIComponet;
typedef class ACubeMap;
UCLASS()
class THETAALGORITHM_API AMainCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainCharacter();
	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	AMapManager* Map;
	UAIComponet* Algorithm;

	TArray<ACubeMap*> Path;
	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	float speed;
	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	FVector Target;
	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	int mul;
	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	float Cooldown;
	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	float Cooldownmax;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Wander(float DeltaTime);
};
