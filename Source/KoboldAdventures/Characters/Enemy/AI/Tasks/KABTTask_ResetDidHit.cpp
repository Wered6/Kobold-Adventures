// Fill out your copyright notice in the Description page of Project Settings.


#include "KABTTask_ResetDidHit.h"
#include "KoboldAdventures/Characters/Enemy/AI/Controller/KAEnemyAIController.h"

EBTNodeResult::Type UKABTTask_ResetDidHit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AKAEnemyAIController* KAEnemyAIC{Cast<AKAEnemyAIController>(OwnerComp.GetAIOwner())};

#pragma region NullChecks
	if (!KAEnemyAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_ResetDidHit::ExecuteTask|KAEnemyAIC is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	KAEnemyAIC->SetDidHit(false);

	return EBTNodeResult::Succeeded;
}
