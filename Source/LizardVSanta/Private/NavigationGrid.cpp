// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigationGrid.h"
#include "LVSFragments.h"
#include "MassEntityTemplateRegistry.h"
#include "MassEntityView.h"
#include "GridActorComponent.h"
namespace FMassAgentTraitsHelper
{
	template<typename T>
	T* AsComponent(UObject& Owner)
	{
		T* Component = nullptr;
		if (AActor* AsActor = Cast<AActor>(&Owner))
		{
			Component = AsActor->FindComponentByClass<T>();
		}
		else
		{
			Component = Cast<T>(&Owner);
		}

		/*UE_CVLOG_UELOG(Component == nullptr, &Owner, LogMass, Error, TEXT("Trying to extract %s from %s failed")
			, *T::StaticClass()->GetName(), *Owner.GetName());*/

		return Component;
	}
}
void UNavigationGrid::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	//BuildContext.AddFragment<FRecastNavMeshFragment>();
	//BuildContext.GetMutableObjectFragmentInitializers().Add([=](UObject& Owner, FMassEntityView& EntityView, const EMassTranslationDirection CurrentDirection)
	//	{
	//		if (ARecastNavMesh* navmesh = FMassAgentTraitsHelper::AsComponent<ARecastNavMesh>(Owner))
	//		{
	//			FRecastNavMeshFragment& navFragment = EntityView.GetFragmentData<FRecastNavMeshFragment>();
	//			navFragment.NavMesh = navmesh;
	//		}
	//	});
}