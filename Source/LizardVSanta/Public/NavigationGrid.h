// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "MassAgentTraits.h"
#include "NavigationGrid.generated.h"
/**
 * 
 */
UCLASS()
class LIZARDVSANTA_API UNavigationGrid : public UMassAgentSyncTrait
{
	GENERATED_BODY()
protected:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
};
