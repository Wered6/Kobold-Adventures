// Fill out your copyright notice in the Description page of Project Settings.


#include "KABTTask_MoveAlongPatrolRoute.h"
#include "AIController.h"
#include "KoboldAdventures/Characters/Enemy/AI/Controller/KAEnemyAIController.h"

EBTNodeResult::Type UKABTTask_MoveAlongPatrolRoute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	KAEnemyAIC = Cast<AKAEnemyAIController>(OwnerComp.GetAIOwner());

#pragma region NullChecks
	if (!KAEnemyAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveAlongPatrolRoute::ExecuteTask|AIC is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma  endregion

	KAEnemyAIC->SetMoveAlongPatrolRouteBTComponent(&OwnerComp);
	KAEnemyAIC->SetMoveAlongPatrolRouteBTTaskNode(this);
	KAEnemyAIC->MoveAlongPatrolRoute();

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UKABTTask_MoveAlongPatrolRoute::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
#pragma region NullChecks
	if (!KAEnemyAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveAlongPatrolRoute::AbortTask|AIC is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	KAEnemyAIC->StopMovement();

	return EBTNodeResult::Aborted;
}
