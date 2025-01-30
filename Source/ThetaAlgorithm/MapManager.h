// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapManager.generated.h"

typedef class ACubeMap;
typedef class AGrid;
UCLASS()
class THETAALGORITHM_API AMapManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapManager();

	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	int m_MapHeight = 100;
	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	int m_MapWidth = 100;
	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	float m_ObstacleDensity = 0.05f;
	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	int m_MaxWallLength = 10;
	UPROPERTY(EditAnywhere, Category = "ActorSpawning")
	int m_MinWallLength = 3;
	AGrid* Grid;
	bool MapSet;
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
	int Distance = 10;
	UPROPERTY(EditDefaultsOnly, Category = "ActorSpawning")
	TSubclassOf<ACubeMap> UsefulActorBP;
	TArray<ACubeMap*> AuxArrayMap;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddRandomObstacle(const int maxWallLength, const int minWallLength, const int height, const int width, int& numBlockedCellsRemaining);
	int GenerateRandomInRange(const int min, const int max);
	void InitCellPosition();

	void GenerateMap();

	ACubeMap* GetCellAt(const int x, const int y) const;
	void GetNeighboursAt(const int CenterX, const int CenterY, TArray<ACubeMap*>& neighbours) const;
	void GetNeighbours(const ACubeMap& centerCell, TArray<ACubeMap*>& neighbours) const;
};
