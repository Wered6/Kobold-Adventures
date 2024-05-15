// Fill out your copyright notice in the Description page of Project Settings.


#include "KABTTask_MoveToTarget.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UKABTTask_MoveToTarget::UKABTTask_MoveToTarget()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UKABTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIC = OwnerComp.GetAIOwner();
	const UBlackboardComponent* BlackboardComp{OwnerComp.GetBlackboardComponent()};

#pragma region NullChecks
	if (!AIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveToTarget::ExecuteTask|AIC is nullptr"))
		return EBTNodeResult::Failed;
	}
	if (!BlackboardComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveToTarget::ExecuteTask|BlackboardComp is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	APawn* AIPawn{AIC->GetPawn()};
	// Get the target location from the blackboard
	AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(BlackboardKey.SelectedKeyName));

#pragma region NullChecks
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveToTarget::ExecuteTask|AIPawn is nullptr"))
		return EBTNodeResult::Failed;
	}
	if (!AttackTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveToTarget::ExecuteTask|AttackTarget is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	const FVector TargetLocation{AttackTarget->GetActorLocation()};
	const FVector CurrentLocation{AIPawn->GetActorLocation()};
	const float DistanceToTarget{static_cast<float>(FVector::Dist(CurrentLocation, TargetLocation))};

	// Check if already within the acceptable radius plus buffer
	if (DistanceToTarget <= AcceptableRadius + 50.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveToTarget::ExecuteTask Succeeded 57 line"))
		return EBTNodeResult::Succeeded;
	}

	// Move to the target location with the acceptable radius
	FNavLocation NavLocation;
	UNavigationSystemV1* NavSys{UNavigationSystemV1::GetCurrent(GetWorld())};

#pragma region NullChecks
	if (!NavSys)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveToTarget::ExecuteTask|NavSys is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	if (NavSys->ProjectPointToNavigation(TargetLocation, NavLocation))
	{
		const EPathFollowingRequestResult::Type MoveToResult{
			AIC->MoveToLocation(NavLocation.Location, AcceptableRadius)
		};
		AIPawn->SetActorRotation(Get2DRotation(CurrentLocation, TargetLocation));

		// Check if we can move to the location
		if (MoveToResult == EPathFollowingRequestResult::RequestSuccessful)
		{
			// Reset the elapsed time counter
			ElapsedTime = 0.f;

			UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveToTarget::ExecuteTask InProgress 86 line"))
			return EBTNodeResult::InProgress;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveToTarget::ExecuteTask Failed 91 line"))
	return EBTNodeResult::Failed;
}

void UKABTTask_MoveToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
#pragma region NullChecks
	if (!AIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveToTarget::TickTask:AIC is nullptr"))
		return;
	}
	if (!AttackTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveToTarget::TickTask|AttackTarget is nullptr"))
		return;
	}
#pragma endregion

	APawn* AIPawn{AIC->GetPawn()};

#pragma region NullChecks
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveToTarget::TickTask|AIPawn is nullptr"))
		return;
	}
#pragma endregion

	const FVector TargetLocation{AttackTarget->GetActorLocation()};
	const FVector CurrentLocation{AIPawn->GetActorLocation()};
	const float DistanceToTarget{static_cast<float>(FVector::Dist(CurrentLocation, TargetLocation))};

	// Finish the task if within the acceptable radius plus buffer
	if (DistanceToTarget <= AcceptableRadius + 50.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveToTarget::TickTask Succeeded 127 line"))
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// Fail the task if the timeout is reached
	if (ElapsedTime >= PathfindingTimeout)
	{
		UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveToTarget::TickTask Failed 138 line"))
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// If not within range, update the AI's destination
	if (AIC->GetPathFollowingComponent()->GetStatus() != EPathFollowingStatus::Moving)
	{
		// Increment the elepased time
		ElapsedTime += DeltaSeconds;

		const EPathFollowingRequestResult::Type MoveToResult{AIC->MoveToLocation(TargetLocation, AcceptableRadius)};
		AIPawn->SetActorRotation(Get2DRotation(CurrentLocation, TargetLocation));

		if (MoveToResult != EPathFollowingRequestResult::RequestSuccessful)
		{
			UE_LOG(LogTemp, Warning, TEXT("UKABTTask_MoveToTarget::TickTask Failed 151 line"))
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
}

FRotator UKABTTask_MoveToTarget::Get2DRotation(const FVector& CurrentLocation, const FVector& TargetLocation) const
{
	// Calculate the dirction to the target
	const FVector Direction{TargetLocation - CurrentLocation};

	// Determine the rotation (0 or 180 degrees)
	const float YawRotation{Direction.X > 0 ? 0.f : 180.f};

	// Set the rotation
	const FRotator NewRotation{FRotator(0.f, YawRotation, 0.f)};

	return NewRotation;
}
