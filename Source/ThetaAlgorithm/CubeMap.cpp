// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeMap.h"

// Sets default values
ACubeMap::ACubeMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	HValue = 1;
	Cost = 1;
	GValue = 99999999;
	FValue = 99999999;
}

// Called when the game starts or when spawned
void ACubeMap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubeMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACubeMap::SetNode(float g, float f, float h, float cost)
{
	GValue = g;
	FValue = f;
	HValue = h;
	Cost = cost;

	TArray<AAIEdge*>();
}

