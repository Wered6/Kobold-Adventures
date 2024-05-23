// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlueprintBase.h"
#include "KABTDecorator_HasFocus.generated.h"

UCLASS()
class KOBOLDADVENTURES_API UKABTDecorator_HasFocus : public UBTDecorator_BlueprintBase
{
	GENERATED_BODY()

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
