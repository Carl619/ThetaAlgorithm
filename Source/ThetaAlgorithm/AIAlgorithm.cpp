// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAlgorithm.h"
#include "CubeMap.h"
#include "Algo/Reverse.h"
#include "AIEdge.h"

// Sets default values
AAIAlgorithm::AAIAlgorithm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Nodes = TArray<ACubeMap*>();
}

// Called when the game starts or when spawned
void AAIAlgorithm::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIAlgorithm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<ACubeMap*>  AAIAlgorithm::Search(int start, int goal)
{
    TMap<int, ACubeMap*> openSet = TMap<int, ACubeMap*>();

    TMap<int, ACubeMap*> closeSet = TMap<int, ACubeMap*>();

    TMap<int, ACubeMap*> cameFrom = TMap<int, ACubeMap*>();


    CalculateH(goal);

    Nodes[start]->GValue = 0;

    Nodes[start]->FValue = Nodes[start]->HValue;



    openSet.Add(Nodes[start]->ID, Nodes[start]);



    while (!openSet.IsEmpty())

    {

        ACubeMap* current = LowestFvalue(openSet);

        if (current == Nodes[goal])

        {

            return Reconstruct(current->ID, Nodes[start]->ID, current, cameFrom);

        }



        openSet.Remove(current->ID);

        for (int i = 0; i < current->Edges.Num(); i++)

        {

            if (closeSet.Find(current->Edges[i]->NodeEnd->ID) == nullptr)

            {

                float tentativeGScore = current->GValue + current->Edges[i]->Cost;

                if (tentativeGScore < current->Edges[i]->NodeEnd->GValue)

                {

                    if (cameFrom.Find(current->Edges[i]->NodeEnd->ID) != nullptr)

                    {

                        cameFrom[current->Edges[i]->NodeEnd->ID] = current;

                    }

                    else {

                        cameFrom.Add(current->Edges[i]->NodeEnd->ID, current);

                    }

                    if (openSet.Find(current->Edges[i]->NodeEnd->ID) == nullptr)

                    {

                        current->Edges[i]->NodeEnd->GValue = tentativeGScore;

                        current->Edges[i]->NodeEnd->FValue = tentativeGScore + current->Edges[i]->NodeEnd->HValue;

                        openSet.Add(current->Edges[i]->NodeEnd->ID, current->Edges[i]->NodeEnd);

                        closeSet.Add(current->ID, current);

                    }

                    else

                    {
                        if (LineOfSigth(cameFrom[current->ID], current->Edges[i]->NodeEnd))
                        { 
                            if (cameFrom[current->ID]->GValue + (cameFrom[current->ID]->GetActorLocation() - current->Edges[i]->NodeEnd->GetActorLocation()).Length() < current->Edges[i]->NodeEnd->GValue)
                            {
                                current->Edges[i]->NodeEnd->GValue = cameFrom[current->ID]->GValue + (cameFrom[current->ID]->GetActorLocation() - current->Edges[i]->NodeEnd->GetActorLocation()).Length();
                                cameFrom.Add(current->Edges[i]->NodeEnd->ID, cameFrom[current->ID]);
                            }
                        }
                        else
                        {
                            if (tentativeGScore < current->Edges[i]->NodeEnd->GValue)

                            {

                                current->Edges[i]->NodeEnd->GValue = tentativeGScore;

                                current->Edges[i]->NodeEnd->FValue = tentativeGScore + current->Edges[i]->NodeEnd->HValue;

                                cameFrom.Add(current->Edges[i]->NodeEnd->ID, current);

                            }
                        }

                    }

                }

            }

        }
    }

    return TArray<ACubeMap*>();
}

bool  AAIAlgorithm::LineOfSigth(ACubeMap* origin, ACubeMap* objective)
{
    // FHitResult will hold all data returned by our line collision query
    FHitResult Hit;

    // We set up a line trace from our current location to a point 1000cm ahead of us
    FVector TraceStart = origin->GetActorLocation() + FVector(0,0,2);
    FVector TraceEnd = objective->GetActorLocation() + FVector(0, 0, 2);

    // You can use FCollisionQueryParams to further configure the query
    // Here we add ourselves to the ignored list so we won't block the trace
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    // To run the query, you need a pointer to the current level, which you can get from an Actor with GetWorld()
    // UWorld()->LineTraceSingleByChannel runs a line trace and returns the first actor hit over the provided collision channel.
    GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_GameTraceChannel1, QueryParams);

    // You can use DrawDebug helpers and the log to help visualize and debug your trace queries.
    DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
    UE_LOG(LogTemp, Log, TEXT("Tracing line: %s to %s"), *TraceStart.ToCompactString(), *TraceEnd.ToCompactString());

    // If the trace hit something, bBlockingHit will be true,
    // and its fields will be filled with detailed info about what was hit
    if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
    {
        ACubeMap* hitActor = (ACubeMap*)Hit.GetActor();
        if (hitActor != nullptr)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else {
        return true;
    }
}

TArray<ACubeMap*> AAIAlgorithm::Reconstruct(int current, int start, ACubeMap* currentNode, TMap<int, ACubeMap*> cameFrom)
{
    TArray<ACubeMap*> path = TArray<ACubeMap*>();

    path.Add(currentNode);



    while (current != start)

    {

        path.Add(cameFrom[current]);

        current = cameFrom[current]->ID;

    }
    return ReverseList(path);
}

ACubeMap* AAIAlgorithm::LowestFvalue(TMap<int, ACubeMap*> nodes)
{
    float fValue = -1;
    ACubeMap* node = nullptr;
    for (const TPair<int, ACubeMap*>& pair : nodes)
    {
        if (fValue == -1)
        {
            node = pair.Value;
        }
        if (pair.Value->FValue < node->FValue)
        {
            node = pair.Value;
        }
    }
    return node;
}
int  AAIAlgorithm::NearestNodeID(FVector location) {
    FVector nearestLocation = Nodes[0]->GetActorLocation();
    int node = 0;
    for (int i = 0; i < Nodes.Num(); i++)
    {
        if (FVector::Dist(nearestLocation, location) > FVector::Dist(Nodes[i]->GetActorLocation(), location))
        {
            nearestLocation = Nodes[i]->GetActorLocation();
            node = i;
        }
    }
    return node;
}
TArray<ACubeMap*>  AAIAlgorithm::ReverseList(TArray<ACubeMap*> nodes)
{
    Algo::Reverse(nodes);
    return nodes;
}
void  AAIAlgorithm::Restart() {
    for (int i = 0; i < Nodes.Num(); i++)
    {
        Nodes[i]->FValue = 99999999;
        Nodes[i]->GValue = 99999999;
    }
}
void  AAIAlgorithm::CalculateH(int goal) {
    for (int i = 0; i < Nodes.Num(); i++)
    {
        if (i != goal)
        {
            Nodes[i]->HValue = FVector::Dist(Nodes[i]->GetActorLocation(), Nodes[goal]->GetActorLocation());
        }
        else {
            Nodes[goal]->HValue = 0;
        }
    }
}
