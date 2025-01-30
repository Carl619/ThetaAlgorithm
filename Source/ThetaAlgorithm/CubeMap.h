// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "CubeMap.generated.h"

typedef class AAIEdge;

UCLASS()
class THETAALGORITHM_API ACubeMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubeMap();
	bool m_IsBlocked;
	int m_PositionX;
	int m_PositionY;
	FVector Node;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float width = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float height = 0.5f;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* SceneRoot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AAIEdge*> Edges;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float GValue;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float FValue;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float HValue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Cost;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ID;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetNode(float g, float f, float h, float cost);
};
