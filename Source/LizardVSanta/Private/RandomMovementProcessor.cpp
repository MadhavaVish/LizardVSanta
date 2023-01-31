// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomMovementProcessor.h"
#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "MassNavigationFragments.h"
#include "NavigationSystem.h"
#include "MassMovement/Public/MassMovementFragments.h"
#include "LVSFragments.h"
#include "Engine/World.h"

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
	//EntityQuery.AddConstSharedRequirement<FNavSystem>(EMassFragmentPresence::All);
	EntityQuery.RegisterWithProcessor(*this);
}

void URandomMovementProcessor::Initialize(UObject& Owner)
{
	Super::Initialize(Owner);
	World = Owner.GetWorld();

	PathFinding = UWorld::GetSubsystem<UPathFindingSubsystem>(World);
}

void URandomMovementProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	
	EntityQuery.ForEachEntityChunk(EntityManager, Context,
		[&, this](FMassExecutionContext& Context)
		{
			const TArrayView<FTransformFragment> TransformList = Context.GetMutableFragmentView<FTransformFragment>();
			const TArrayView<FMassMoveTargetFragment> NavTargetList = Context.GetMutableFragmentView<FMassMoveTargetFragment>();
			const FMassMovementParameters& MovementParams = Context.GetConstSharedFragment<FMassMovementParameters>();

			for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
			{
				FTransform& Transform = TransformList[EntityIndex].GetMutableTransform();
				FMassMoveTargetFragment& MoveTarget = NavTargetList[EntityIndex];

				FVector CurrentLocation = Transform.GetLocation();
				FVector TargetVector = MoveTarget.Center - CurrentLocation;

				if (MoveTarget.Center == FVector::ZeroVector)
				{
					MoveTarget.Center = FVector(6000.f, 0.f, CurrentLocation.Z);
					MoveTarget.DesiredSpeed = FMassInt16Real(MovementParams.DefaultDesiredSpeed);
				}				
				//MoveTarget.Center = FVector(6000.f, 0.f, 1.f);
				MoveTarget.DistanceToGoal = (MoveTarget.Center - Transform.GetLocation()).Size();
				MoveTarget.Forward = (MoveTarget.Center - Transform.GetLocation()).GetSafeNormal();
				if (MoveTarget.DistanceToGoal < 1.f)
				{
					MoveTarget.Center = PathFinding->GetNextPathPoint(CurrentLocation, FVector(12000.f, 0.f, 1.f));
				}
			}
			
		});
}