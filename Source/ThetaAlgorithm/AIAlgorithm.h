// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIAlgorithm.generated.h"

typedef class ACubeMap;


UCLASS()
class THETAALGORITHM_API AAIAlgorithm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIAlgorithm();

	TArray<ACubeMap*> Nodes;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<ACubeMap*> Reconstruct(int current, int start, ACubeMap* currentNode, TMap<int, ACubeMap*> cameFrom);
	TArray<ACubeMap*> Search(int start, int goal);
	ACubeMap* LowestFvalue(TMap<int, ACubeMap*> nodes);
	int NearestNodeID(FVector location);
	TArray<ACubeMap*> ReverseList(TArray<ACubeMap*> nodes);
	void Restart();
	void CalculateH(int goal);
	bool LineOfSigth(ACubeMap* origin, ACubeMap* objective);
};
