// Fill out your copyright notice in the Description page of Project Settings.


#include "KABTTask_DefaultAttack.h"
#include "AIController.h"
#include "KoboldAdventures/Characters/Enemy/KAEnemy.h"
#include "KoboldAdventures/Characters/Enemy/AI/Controller/KAEnemyAIController.h"

EBTNodeResult::Type UKABTTask_DefaultAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AKAEnemyAIController* KAEnemyAIC{Cast<AKAEnemyAIController>(OwnerComp.GetAIOwner())};

#pragma region NullChecks
	if (!KAEnemyAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_AttackTarget::ExecuteTask|AIC is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	KAEnemyAIC->SetDefaultAttackBTComponent(&OwnerComp);
	KAEnemyAIC->SetDefaultAttackBTTaskNode(this);

	const AKAEnemy* Enemy{Cast<AKAEnemy>(KAEnemyAIC->GetPawn())};

#pragma region NullChecks
	if (!Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_AttackTarget::ExecuteTask|Enemy is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	Enemy->Attack();

	return EBTNodeResult::InProgress;
}
