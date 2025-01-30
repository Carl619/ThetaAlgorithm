// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIComponet.generated.h"

typedef class ACubeMap;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THETAALGORITHM_API UAIComponet : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIComponet();
	TArray<ACubeMap*> Nodes;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<ACubeMap*> Reconstruct(int current, int start, ACubeMap* currentNode, TMap<int, ACubeMap*> cameFrom);
	TArray<ACubeMap*> Search(int start, int goal);
	ACubeMap* LowestFvalue(TMap<int, ACubeMap*> nodes);
	int NearestNodeID(FVector location);
	TArray<ACubeMap*> ReverseList(TArray<ACubeMap*> nodes);
	void Restart();
	void CalculateH(int goal);
	bool LineOfSigth(ACubeMap* origin, ACubeMap* objective);
		
};
