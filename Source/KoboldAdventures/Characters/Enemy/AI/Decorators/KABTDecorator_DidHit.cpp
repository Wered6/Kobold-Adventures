// Fill out your copyright notice in the Description page of Project Settings.


#include "KABTDecorator_DidHit.h"
#include "KoboldAdventures/Characters/Enemy/AI/Controller/KAEnemyAIController.h"

bool UKABTDecorator_DidHit::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AKAEnemyAIController* KAEnemyAIC{Cast<AKAEnemyAIController>(OwnerComp.GetAIOwner())};

#pragma region NullChecks
	if (!KAEnemyAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTDecorator_DidHit::CalculateRawConditionValue|KAEnemyAIC is nullptr"))
		return false;
	}
#pragma endregion
	
	return KAEnemyAIC->GetDidHit();
}
