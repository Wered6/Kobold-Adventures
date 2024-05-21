// Fill out your copyright notice in the Description page of Project Settings.


#include "KAPatrolRoute.h"
#include "Components/SplineComponent.h"

AKAPatrolRoute::AKAPatrolRoute()
{
	PatrolRoute = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolRoute"));
	PatrolRoute->SetupAttachment(RootComponent);
}

FVector AKAPatrolRoute::GetSplinePointAsWorldPosition() const
{
#pragma region NullChecks
	if (!PatrolRoute)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAPatrolRoute::GetSplintePointAsWorldPosition|PatrolRoute is nullptr"))
		return FVector::ZeroVector;
	}
#pragma endregion

	const FVector SplinePointLocation{
		PatrolRoute->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World)
	};

	return SplinePointLocation;
}

void AKAPatrolRoute::IncrementPatrolRoute()
{
#pragma region NullChecks
	if (!PatrolRoute)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAPatrolRoute::IncrementPatrolRoute|PatrolRoute is nullptr"))
		return;
	}
#pragma endregion
	
	PatrolIndex += Direction;
	const int32 LastPatrolIndex{PatrolRoute->GetNumberOfSplinePoints() - 1};

	if (PatrolIndex == LastPatrolIndex)
	{
		Direction = -1;
	}
	else if (PatrolIndex == 0)
	{
		Direction = 1;
	}
}
