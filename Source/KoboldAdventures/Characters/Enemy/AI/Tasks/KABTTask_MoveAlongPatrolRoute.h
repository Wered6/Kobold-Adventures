// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "KABTTask_MoveAlongPatrolRoute.generated.h"

class AKAPatrolRoute;
struct FPathFollowingResult;

UCLASS()
class KOBOLDADVENTURES_API UKABTTask_MoveAlongPatrolRoute : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, const EPathFollowingResult::Type Result);

	UPROPERTY(VisibleAnywhere, Category="KA")
	TObjectPtr<AKAPatrolRoute> PatrolRoute;
	UPROPERTY(VisibleAnywhere, Category="KA")
	TObjectPtr<UBehaviorTreeComponent> BTComponent;

	UPROPERTY()
	TObjectPtr<AAIController> AIC;
};
