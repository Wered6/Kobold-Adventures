// Fill out your copyright notice in the Description page of Project Settings.


#include "KABTTask_ClearFocus.h"
#include "KoboldAdventures/Characters/Enemy/AI/Controller/KAEnemyAIController.h"

EBTNodeResult::Type UKABTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AKAEnemyAIController* KAEnemyAIC{Cast<AKAEnemyAIController>(OwnerComp.GetAIOwner())};

#pragma region NullChecks
	if (!KAEnemyAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_ClearFocus::ExecuteTask|KAEnemyAIC is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	KAEnemyAIC->SetHasFocus(false);

	return EBTNodeResult::Succeeded;
}
