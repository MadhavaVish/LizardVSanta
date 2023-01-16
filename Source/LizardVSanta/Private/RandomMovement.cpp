// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomMovement.h"
#include "LVSFragments.h"
#include "MassEntityTemplateRegistry.h"
void URandomMovement::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment<FRandomTarget>();
}
