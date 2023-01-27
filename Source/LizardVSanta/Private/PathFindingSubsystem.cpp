// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFindingSubsystem.h"
#include "NavigationPath.h"
#include "NavigationSystemTypes.h"
UPathFindingSubsystem::UPathFindingSubsystem()
{
	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
}

FVector UPathFindingSubsystem::GetNextPathPoint(FVector start, FVector end)
{
	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	const FNavAgentProperties& props = FNavAgentProperties(5000.f);
	const ANavigationData* NavigationData = NavSys->GetNavDataForProps(props, start);
	const FPathFindingQuery Query(NavSys, *NavigationData, start, end, UNavigationQueryFilter::GetQueryFilter(*NavigationData, NULL, NULL));
	const FPathFindingResult Result = NavSys->FindPathSync(Query, EPathFindingMode::Regular);

	/*path->MarkAsGarbage();
	path = NavSys->FindPathToLocationSynchronously(GetWorld(), start, end, NULL);
	if (!path->PathPoints.IsEmpty())
	{
		return FVector(path->PathPoints[1]);
	}
	return end;*/
	
	if (Result.IsSuccessful())
	{
		auto& pts = Result.Path->GetPathPoints();
		if (pts.IsValidIndex(1))
		{

			return pts[1];
		}
	}
	return end;
}

void UPathFindingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
}
