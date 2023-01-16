// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MassCommon/Public/MassCommonFragments.h"
#include "PathFindingSubsystem.h"
#include "RandomMovementProcessor.generated.h"
/**
 * 
 */
UCLASS()
class LIZARDVSANTA_API URandomMovementProcessor : public UMassProcessor
{
	GENERATED_BODY()
public:
	URandomMovementProcessor();

protected:
	virtual void Initialize(UObject& Owner) override;
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

	TObjectPtr<UWorld> World;
	TObjectPtr<UPathFindingSubsystem> PathFinding;
	FMassEntityQuery EntityQuery;
};
