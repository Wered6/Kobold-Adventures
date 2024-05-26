// Fill out your copyright notice in the Description page of Project Settings.


#include "KABTTask_SetStateAsPassive.h"
#include "KoboldAdventures/Characters/Enemy/AI/Controller/KAEnemyAIController.h"

EBTNodeResult::Type UKABTTask_SetStateAsPassive::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AKAEnemyAIController* KAEnemyAIC{Cast<AKAEnemyAIController>(OwnerComp.GetAIOwner())};

#pragma region NullChecks
	if (!KAEnemyAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_SetStateAsPassive::ExecuteTask|KAEnemyAIC is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	KAEnemyAIC->SetStateAsPassive();

	return EBTNodeResult::Succeeded;
}
