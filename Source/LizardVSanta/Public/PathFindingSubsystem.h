// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "NavigationSystem.h"
#include "MassExternalSubsystemTraits.h"
#include "PathFindingSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class LIZARDVSANTA_API UPathFindingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UPathFindingSubsystem();
	FVector GetNextPathPoint(FVector start, FVector end);
protected:
	// UWorldSubsystem begin interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
private:
	UNavigationSystemV1* NavSys;
	UNavigationPath* path;
};

template<>
struct TMassExternalSubsystemTraits<UPathFindingSubsystem> final
{
	enum
	{
		ThreadSafeRead = true,
		ThreadSafeWrite = false,
	};
};