// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CubeMap.h"
#include "Obstacle.generated.h"

/**
 * 
 */
UCLASS()
class THETAALGORITHM_API AObstacle : public ACubeMap
{
	GENERATED_BODY()
	
	int width = 5;
	int height = 5;
};
