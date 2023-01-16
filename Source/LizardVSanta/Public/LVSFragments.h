// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassCommonFragments.h"
#include "MassEntityTypes.h"
#include "MassCommonTypes.h"
#include "GridActorComponent.h"
#include "Navmesh/RecastNavMesh.h"
#include "LVSFragments.generated.h"
/**
 * 
 */

//class UGridActorComponent;

USTRUCT()
struct LIZARDVSANTA_API FRandomTarget : public FMassFragment
{
	GENERATED_BODY()

	FVector Target;
};

USTRUCT()

struct LIZARDVSANTA_API FGridNavigableFragment : public FObjectWrapperFragment
{
	GENERATED_BODY()
	TWeakObjectPtr<UGridActorComponent> Grid;
};

USTRUCT()
struct LIZARDVSANTA_API FRecastNavMeshFragment : public FObjectWrapperFragment
{
	GENERATED_BODY()
	TWeakObjectPtr<ARecastNavMesh> NavMesh;
};

USTRUCT()
struct LIZARDVSANTA_API FNavSystem : public FMassSharedFragment
{
	GENERATED_BODY()
	TWeakObjectPtr<UNavigationSystemV1> NavSys;
};