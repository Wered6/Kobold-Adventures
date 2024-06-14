// Fill out your copyright notice in the Description page of Project Settings.


#include "UKABTTask_Stun.h"
#include "KoboldAdventures/Characters/Enemy/AI/Controller/KAEnemyAIController.h"
#include "KoboldAdventures/Characters/Enemy/Mushroom/KAMushroom.h"

EBTNodeResult::Type UUKABTTask_Stun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AKAEnemyAIController* KAEnemyAIC{Cast<AKAEnemyAIController>(OwnerComp.GetAIOwner())};

#pragma region NullChecks
	if (!KAEnemyAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UUKABTTask_Stun::ExecuteTask|KAEnemyAIC is nullptr"))
		return EBTNodeResult::Failed; 
	}
#pragma endregion 

	KAEnemyAIC->SetStunBTComponent(&OwnerComp);
	KAEnemyAIC->SetStunBTTaskNode(this);
	
	AKAMushroom* Mushroom{Cast<AKAMushroom>(KAEnemyAIC->GetPawn())};

#pragma region NullChecks
	if (!Mushroom)
	{
		UE_LOG(LogTemp, Warning, TEXT("UUKABTTask_Stun::ExecuteTask|Mushroom is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	Mushroom->Stun();

	return EBTNodeResult::InProgress;
}
