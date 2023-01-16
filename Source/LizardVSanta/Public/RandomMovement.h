// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "RandomMovement.generated.h"

/**
 * 
 */
UCLASS()
class   URandomMovement : public UMassEntityTraitBase
{
	GENERATED_BODY()
protected:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
};
