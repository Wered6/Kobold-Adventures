// Fill out your copyright notice in the Description page of Project Settings.


#include "KABTDecorator_HasFocus.h"
#include "KoboldAdventures/Characters/Enemy/AI/Controller/KAEnemyAIController.h"

bool UKABTDecorator_HasFocus::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AKAEnemyAIController* KAEnemyAIC{Cast<AKAEnemyAIController>(OwnerComp.GetAIOwner())};

#pragma region NullChecks
	if (!KAEnemyAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTDecorator_HasFocus::CalculateRawConditionValue|KAAIC is nullptr"))
		return false;
	}
#pragma endregion

	return KAEnemyAIC->GetHasFocus();
}
