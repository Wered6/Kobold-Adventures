// Fill out your copyright notice in the Description page of Project Settings.


#include "KABTTask_MoveAlongPatrolRoute.h"
#include "AIController.h"
#include "KoboldAdventures/Characters/Enemy/AI/KAEnemyAIInterface.h"
#include "KoboldAdventures/Characters/Enemy/AI/KAPatrolRoute.h"
#include "Navigation/PathFollowingComponent.h"

EBTNodeResult::Type UKABTTask_MoveAlongPatrolRoute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIC = OwnerComp.GetAIOwner();

#pragma region NullChecks
	if (!AIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveAlongPatrolRoute::ExecuteTask|AIC is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma  endregion

	APawn* AIPawn{AIC->GetPawn()};

#pragma region NullChecks
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveAlongPatrolRoute::ExecuteTask|AIPawn is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	const bool bImplementsInterface{AIPawn->Implements<UKAEnemyAIInterface>()};
	if (bImplementsInterface)
	{
		PatrolRoute = IKAEnemyAIInterface::Execute_GetPatrolRoute(AIPawn);
	}

#pragma region NullChecks
	if (!PatrolRoute)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveAlongPatrolRoute::ExecuteTask|PatrolRoute is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	const FVector Destination{PatrolRoute->GetSplinePointAsWorldPosition()};
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(Destination);
	MoveRequest.SetAcceptanceRadius(10.f);

	FNavPathSharedPtr NavPath;
	AIC->MoveTo(MoveRequest, &NavPath);
	AIC->ReceiveMoveCompleted.AddDynamic(this, &UKABTTask_MoveAlongPatrolRoute::OnMoveCompleted);

	BTComponent = &OwnerComp;

	return EBTNodeResult::InProgress;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UKABTTask_MoveAlongPatrolRoute::OnMoveCompleted(FAIRequestID RequestID, const EPathFollowingResult::Type Result)
{
#pragma region NullChecks
	if (!AIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveAlongPatrolRoute::OnMoveCompleted|AIC is nullptr"))
		return;
	}
	if (!PatrolRoute)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveAlongPatrolRoute::OnMoveCompleted|PatrolRoute is nullptr"))
		return;
	}
	if (!BTComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveAlongPatrolRoute::OnMoveCompleted|BTComponent is nullptr"))
		return;
	}
#pragma endregion

	AIC->ReceiveMoveCompleted.RemoveAll(this);

	if (Result == EPathFollowingResult::Success)
	{
		PatrolRoute->IncrementPatrolRoute();
		BTComponent->OnTaskFinished(this, EBTNodeResult::Succeeded);
	}
}
