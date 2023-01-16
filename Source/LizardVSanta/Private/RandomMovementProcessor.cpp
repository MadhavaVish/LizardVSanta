// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomMovementProcessor.h"
#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "MassNavigationFragments.h"
#include "NavigationSystem.h"
#include "MassMovement/Public/MassMovementFragments.h"
#include "LVSFragments.h"

URandomMovementProcessor::URandomMovementProcessor()
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)(EProcessorExecutionFlags::All);
	ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::Avoidance);
}

void URandomMovementProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FMassMoveTargetFragment>(EMassFragmentAccess::ReadWrite);
	//EntityQuery.AddRequirement<FRecastNavMeshFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddConstSharedRequirement<FMassMovementParameters>(EMassFragmentPresence::All);
	EntityQuery.RegisterWithProcessor(*this);
}

void URandomMovementProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	
	EntityQuery.ForEachEntityChunk(EntityManager, Context, 
		[&, this](FMassExecutionContext& Context)
		{
			const TArrayView<FTransformFragment> TransformList = Context.GetMutableFragmentView<FTransformFragment>();
			const TArrayView<FMassMoveTargetFragment> NavTargetList = Context.GetMutableFragmentView<FMassMoveTargetFragment>();
			const FMassMovementParameters& MovementParams = Context.GetConstSharedFragment<FMassMovementParameters>();
			//const TArrayView<const FRecastNavMeshFragment> NavFragment = Context.GetFragmentView<FRecastNavMeshFragment>();
			for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
			{
				FTransform& Transform = TransformList[EntityIndex].GetMutableTransform();
				FMassMoveTargetFragment& MoveTarget = NavTargetList[EntityIndex];

				FVector CurrentLocation = Transform.GetLocation();
				FVector TargetVector = MoveTarget.Center - CurrentLocation;

				/*if (MoveTarget.Center == FVector::ZeroVector)
				{
					MoveTarget.Center = FVector(FMath::FRandRange(-1.f, 1.f) * 5000.f, FMath::FRandRange(-1.f, 1.f) * 5000.f, CurrentLocation.Z);
					
				}*/
				/*auto transform = NavFragment[EntityIndex].NavMesh->GetTransform();
				NavFragment[EntityIndex].NavMesh->SetActorScale3D(FVector(20.f));*/
				//TWeakObjectPtr<UNavigationSystemV1> NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
				TWeakObjectPtr <UNavigationSystemV1> NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
				TWeakObjectPtr <UNavigationPath> path = NavSys->FindPathToLocationSynchronously(GetWorld(), CurrentLocation, FVector(6000.f, 0.f, 1.f), NULL);
				
				if (!path->PathPoints.IsEmpty())
				{
					//UE_LOG(TEM)
					MoveTarget.Center = FVector(path->PathPoints[1]);
					//DrawDebugSphere(GetWorld(), path->PathPoints[1], 10.0f, 12, FColor(255, 0, 0));
				}
				MoveTarget.DistanceToGoal = (MoveTarget.Center - Transform.GetLocation()).Size();
				MoveTarget.Forward = (MoveTarget.Center - Transform.GetLocation()).GetSafeNormal();
				MoveTarget.DesiredSpeed = FMassInt16Real(MovementParams.DefaultDesiredSpeed);
				//path->~UNavigationPath();
				//path->destroy
			}
			
		});
}