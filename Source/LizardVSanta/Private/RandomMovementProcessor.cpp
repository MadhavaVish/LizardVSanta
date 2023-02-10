// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomMovementProcessor.h"
#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "MassNavigationFragments.h"
#include "NavigationSystem.h"
#include "MassMovement/Public/MassMovementFragments.h"
#include "LVSFragments.h"
#include "Engine/World.h"
#include "MassDebuggerSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "MassDebugVisualizationComponent.h"
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
	EntityQuery.AddRequirement<FSimDebugVisFragment>(EMassFragmentAccess::ReadOnly);
	//EntityQuery.AddRequirement<FRecastNavMeshFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddConstSharedRequirement<FMassMovementParameters>(EMassFragmentPresence::All);
	//EntityQuery.AddConstSharedRequirement<FNavSystem>(EMassFragmentPresence::All);
	EntityQuery.RegisterWithProcessor(*this);
	EntityQuery.AddSubsystemRequirement<UMassDebuggerSubsystem>(EMassFragmentAccess::ReadWrite);
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
	UMassDebuggerSubsystem& Debugger = Context.GetMutableSubsystemChecked<UMassDebuggerSubsystem>(World);
	UMassDebugVisualizationComponent* Visualizer = Debugger.GetVisualizationComponent();
	//check(Visualizer);
	TArrayView<UHierarchicalInstancedStaticMeshComponent*> VisualDataISMCs = Visualizer->GetVisualDataISMCs();
	const TConstArrayView<FSimDebugVisFragment> DebugVisList = Context.GetFragmentView<FSimDebugVisFragment>();
	for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
	{
		FTransform& Transform = TransformList[EntityIndex].GetMutableTransform();
		FMassMoveTargetFragment& MoveTarget = NavTargetList[EntityIndex];
		const FSimDebugVisFragment& VisualComp = DebugVisList[EntityIndex];
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
		MoveTarget.Center = PathFinding->GetNextPathPoint(CurrentLocation, FVector(15000.f, 15000.f, -100.f));
		FTransform transform(Transform);

		transform.SetScale3D({ 0.0,0.0,0.0 });
		VisualDataISMCs[VisualComp.VisualType]->GetInstanceTransform(VisualComp.InstanceIndex, transform, true);
		auto forward = VisualDataISMCs[VisualComp.VisualType]->GetForwardVector();
		auto newfor = MoveTarget.Center - CurrentLocation;
		double y = newfor.Y - forward.Y;
		double x = newfor.X - forward.X;

		auto angle = atan2(y, x);
		FQuat rot(FVector(0, 0, 1), angle);
		//FTransform tran;
		//transform.TransformPosition(CurrentLocation);
		transform.SetRotation(rot);
		VisualDataISMCs[VisualComp.VisualType]->UpdateInstanceTransform(VisualComp.InstanceIndex, transform, true);

		if ((MoveTarget.Center - FVector(15000.f, 15000.f, -100.f)).Length() < 50)
		{
			Context.Defer().RemoveFragment<FSimDebugVisFragment>(Context.GetEntity(EntityIndex));
			Context.Defer().RemoveTag< FMassDebuggableTag>(Context.GetEntity(EntityIndex));
			EntityManager.Defer().DestroyEntity(Context.GetEntity(EntityIndex));


			//transform.GetRotation();
			//FQuat rot(FVector(0,0,1), )
			//transform.SetRotation()
			VisualDataISMCs[VisualComp.VisualType]->RemoveInstance(VisualComp.InstanceIndex);

		}
	}

		});
}