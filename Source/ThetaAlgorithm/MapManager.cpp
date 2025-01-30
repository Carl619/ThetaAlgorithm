// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"
#include "Grid.h"
#include "CubeMap.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "AIEdge.h"
// Sets default values
AMapManager::AMapManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MapSet = false;
}

// Called when the game starts or when spawned
void AMapManager::BeginPlay()
{
	Super::BeginPlay();
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	Grid = GetWorld()->SpawnActor<AGrid>(SpawnLocation, SpawnRotation);
	int ID = 0;
	for (int i = 0; i < m_MapHeight; i++)
	{
		for (int j = 0; j < m_MapWidth; j++)
		{
			FVector cellLocation = SpawnLocation + FVector(i * Distance , j * Distance, 0);
			ACubeMap* cube = GetWorld()->SpawnActor<ACubeMap>(UsefulActorBP, cellLocation, SpawnRotation);
			cube->ID = ID;
			AuxArrayMap.Add(cube);
			ID++;
		}
	}
}

// Called every frame
void AMapManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!MapSet)
	{
		GenerateMap();
		MapSet = true;
	}
}

void AMapManager::AddRandomObstacle(const int maxWallLength, const int minWallLength, const int height, const int width, int& numBlockedCellsRemaining)
{
	int wallLength = GenerateRandomInRange(minWallLength, maxWallLength);
	int positionX = GenerateRandomInRange(0, width - 1);
	int positionY = GenerateRandomInRange(0, height - 1);
	bool isHorizontalWall = GenerateRandomInRange(0, 1) == 0 ? true : false;

	while (wallLength > 0 && numBlockedCellsRemaining > 0 && positionX < width - 1 && positionY < height - 1)
	{
		Grid->States[positionY][positionX]->m_IsBlocked = true;
		Grid->States[positionY][positionX]->height = 3;
		wallLength--;
		numBlockedCellsRemaining--;
		if (isHorizontalWall)
		{
			positionX++;
		}
		else
		{
			positionY++;
		}
	}
}

int AMapManager::GenerateRandomInRange(const int min, const int max)
{
	return FMath::RandRange(min, max);
}

void AMapManager::InitCellPosition()
{
	for (int y = 0; y < m_MapHeight; y++)
	{
		TArray<ACubeMap*> MapArray = TArray<ACubeMap*>();
		Grid->States.Add(MapArray);
		for (int x = 0; x < m_MapWidth; x++)
		{
			ACubeMap* cube = AuxArrayMap[y * m_MapHeight + x];
			cube->m_PositionX = x;
			cube->m_PositionY = y;
			Grid->States[y].Add(cube);
		}
	}
}

void AMapManager::GenerateMap()
{
	InitCellPosition();

	const int numCells = m_MapHeight * m_MapWidth;
	int numBlockedCellsRemaining = static_cast<int>(m_ObstacleDensity * static_cast<float>(numCells));
	while (numBlockedCellsRemaining > 0)
	{
		AddRandomObstacle(m_MaxWallLength, m_MinWallLength, m_MapHeight, m_MapWidth, numBlockedCellsRemaining);
	}

	for (int i = 0; i < m_MapHeight; i++)
	{
		for (int j = 0; j < m_MapWidth; j++)
		{
			TArray<ACubeMap*> neighbours = TArray<ACubeMap*>();
			GetNeighbours(*Grid->States[i][j], neighbours);
			for (int k = 0; k < neighbours.Num(); k++)
			{
				AAIEdge* edge = NewObject<AAIEdge>(this, AAIEdge::StaticClass());
				edge->NodeEnd = neighbours[k];
				edge->Cost = FVector::Dist(Grid->States[i][j]->GetActorLocation(), neighbours[k]->GetActorLocation());
				Grid->States[i][j]->Edges.Add(edge);
			}
		}
	}
}

ACubeMap* AMapManager::GetCellAt(const int x, const int y) const
{
	check(x < m_MapWidth || y < m_MapHeight);
	return (Grid->States)[y][x];
}

void AMapManager::GetNeighboursAt(const int CenterX, const int CenterY, TArray<ACubeMap*>& neighbours) const
{
	neighbours.Empty();
	check(CenterX < m_MapWidth || CenterY < m_MapHeight);

	if (CenterY > 0)
	{
		if(!GetCellAt(CenterX, CenterY - 1)->m_IsBlocked)
			neighbours.Add(GetCellAt(CenterX, CenterY - 1));
	}
	if (CenterY < m_MapHeight - 1)
	{
		if (!GetCellAt(CenterX, CenterY + 1)->m_IsBlocked)
		neighbours.Add(GetCellAt(CenterX, CenterY + 1));
	}

	if (CenterX > 0)
	{
		if (!GetCellAt(CenterX - 1, CenterY)->m_IsBlocked)
		neighbours.Add(GetCellAt(CenterX - 1, CenterY));

		if (CenterY > 0)
		{
			if (!GetCellAt(CenterX - 1, CenterY - 1)->m_IsBlocked)
			neighbours.Add(GetCellAt(CenterX - 1, CenterY - 1));
		}
		if (CenterY < m_MapHeight - 1)
		{
			if (!GetCellAt(CenterX - 1, CenterY + 1)->m_IsBlocked)
			neighbours.Add(GetCellAt(CenterX - 1, CenterY + 1));
		}
	}

	if (CenterX < m_MapWidth - 1)
	{
		if (!GetCellAt(CenterX + 1, CenterY)->m_IsBlocked)
		neighbours.Add(GetCellAt(CenterX + 1, CenterY));

		if (CenterY > 0)
		{
			if (!GetCellAt(CenterX + 1, CenterY - 1)->m_IsBlocked)
			neighbours.Add(GetCellAt(CenterX + 1, CenterY - 1));
		}
		if (CenterY < m_MapHeight - 1)
		{
			if (!GetCellAt(CenterX + 1, CenterY + 1)->m_IsBlocked)
			neighbours.Add(GetCellAt(CenterX + 1, CenterY + 1));
		}
	}
}

void AMapManager::GetNeighbours(const ACubeMap& centerCell, TArray<ACubeMap*>& neighbours) const
{
	GetNeighboursAt(centerCell.m_PositionX, centerCell.m_PositionY, neighbours);
}

