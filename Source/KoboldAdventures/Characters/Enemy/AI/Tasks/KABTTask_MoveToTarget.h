// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "KABTTask_MoveToTarget.generated.h"

UCLASS()
class KOBOLDADVENTURES_API UKABTTask_MoveToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UKABTTask_MoveToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	FRotator Get2DRotation(const FVector& CurrentLocation, const FVector& TargetLocation) const;
	
	UPROPERTY()
	TObjectPtr<AAIController> AIC;
	
	UPROPERTY()
	TObjectPtr<AActor> AttackTarget;

	UPROPERTY(EditAnywhere, Category="Node")
	float AcceptableRadius{50.f};
	// Time to wait for pathfinding to succeed before failing the task
	UPROPERTY(EditAnywhere, Category="Node")
	float PathfindingTimeout{5.f};
	//Elapsed time since the task started
	float ElapsedTime{0.f};
};
