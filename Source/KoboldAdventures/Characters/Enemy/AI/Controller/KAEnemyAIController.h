// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KAEnemyAIController.generated.h"

class AKAEnemy;

UCLASS()
class KOBOLDADVENTURES_API AKAEnemyAIController : public AAIController
{
	GENERATED_BODY()

private:
	UFUNCTION(BlueprintCallable, Category="KA")
	void SetFocusDirection(AActor* AttackTarget, AKAEnemy* Enemy);
};
