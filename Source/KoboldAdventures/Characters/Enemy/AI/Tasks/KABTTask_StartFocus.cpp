// Fill out your copyright notice in the Description page of Project Settings.


#include "KABTTask_StartFocus.h"
#include "KoboldAdventures/Characters/Enemy/AI/Controller/KAEnemyAIController.h"

EBTNodeResult::Type UKABTTask_StartFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AKAEnemyAIController* KAEnemyAIC{Cast<AKAEnemyAIController>(OwnerComp.GetAIOwner())};

#pragma region NullChecks
	if (!KAEnemyAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_StartFocus::ExecuteTask|KAEnemyAIC is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	KAEnemyAIC->SetHasFocus(true);

	return EBTNodeResult::Succeeded;
}
