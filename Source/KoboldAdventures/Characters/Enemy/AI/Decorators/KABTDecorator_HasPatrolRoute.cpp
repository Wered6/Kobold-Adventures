// Fill out your copyright notice in the Description page of Project Settings.


#include "KABTDecorator_HasPatrolRoute.h"
#include "AIController.h"
#include "KoboldAdventures/Characters/Enemy/AI/KAEnemyAIInterface.h"
#include "KoboldAdventures/Characters/Enemy/AI/KAPatrolRoute.h"

bool UKABTDecorator_HasPatrolRoute::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                               uint8* NodeMemory) const
{
	const AAIController* AIC{OwnerComp.GetAIOwner()};

#pragma region NullChecks
	if (!AIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTDecorator_HasPatrolRoute::CalculateRawConditionValue|AIC is nullptr"))
		return false;
	}
#pragma endregion

	APawn* AIPawn{AIC->GetPawn()};

#pragma region NullChecks
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTDecorator_HasPatrolRoute::CalculateRawConditionValue|AIPawn is nullptr"))
		return false;
	}
#pragma endregion

	bool bHasPatrolRoute{false};
	const bool bImplementsInterface{AIPawn->Implements<UKAEnemyAIInterface>()};
	if (bImplementsInterface)
	{
		const AKAPatrolRoute* PatrolRoute{IKAEnemyAIInterface::Execute_GetPatrolRoute(AIPawn)};
		bHasPatrolRoute = IsValid(PatrolRoute);
	}

	return bHasPatrolRoute;
}
