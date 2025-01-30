// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "MapManager.h"
#include "AIComponet.h"
#include "CubeMap.h"
#include "Grid.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Algorithm = CreateDefaultSubobject<UAIComponet>(TEXT("AI"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (Map->MapSet && Algorithm->Nodes.IsEmpty())
    {
        for (int i = 0; i < Map->m_MapHeight; i++)
        {
            for (int j = 0; j < Map->m_MapWidth; j++)
            {
                Algorithm->Nodes.Add(Map->Grid->States[i][j]);
            }
        }
        int x;
        int y;
        do {
            x = FMath::RandRange(0, 9);
            y = FMath::RandRange(0, 9);
        } while (Map->Grid->States[x][y]->m_IsBlocked);
        FVector playerNode = Map->Grid->States[x][y]->GetActorLocation() + FVector(0, 0, GetActorLocation().Z);
        SetActorLocation(playerNode);
    }
    if (!Algorithm->Nodes.IsEmpty())
    {
        Wander(DeltaTime);
    }
}

void AMainCharacter::Wander(float DeltaTime)
{
    float step = speed * DeltaTime; // calculate distance to move
    Cooldown += DeltaTime;
    if (Path.IsEmpty())
    {
        if (Cooldown > Cooldownmax) {
            Algorithm->Restart();
            int x;
            int y;
            int goal;
            do
            {
                 x = FMath::RandRange(0, 9);
                 y = FMath::RandRange(0, 9);
                 goal = x + y * 10;
            } while (Algorithm->Nodes[goal]->m_IsBlocked);
            Path = Algorithm->Search(Algorithm->NearestNodeID(GetActorLocation()), goal);
            Cooldown = 0;
        }
    }
    if (!Path.IsEmpty())
    {
        FVector goalDist = Path[0]->GetActorLocation() + FVector(0, 0, GetActorLocation().Z);
        FRotator RotTarget = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), goalDist);
        FQuat NewRotation = FQuat::Slerp(GetActorQuat(), RotTarget.Quaternion(), DeltaTime * speed);
        SetActorLocation(FMath::Lerp(GetActorLocation(), Path[0]->GetActorLocation()+FVector(0,0, GetActorLocation().Z), step));
        SetActorRotation(NewRotation);

        
        if (FVector::Dist(goalDist, GetActorLocation()) < 1)
        {
            Path.RemoveAt(0);
        }
    }
}

