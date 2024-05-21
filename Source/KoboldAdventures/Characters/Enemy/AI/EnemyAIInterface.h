// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyAIInterface.generated.h"

class AKAPatrolRoute;

UINTERFACE()
class UEnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};

class KOBOLDADVENTURES_API IEnemyAIInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="KA")
	AKAPatrolRoute* GetPatrolRoute();
};
