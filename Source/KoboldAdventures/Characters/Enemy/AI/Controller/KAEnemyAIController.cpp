// Fill out your copyright notice in the Description page of Project Settings.


#include "KAEnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KoboldAdventures/Characters/Enemy/KAEnemy.h"

void AKAEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AKAEnemy* Enemy{Cast<AKAEnemy>(InPawn)};

#pragma region NullChecks
	if (!Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::OnPossess|Enemy is nullptr"))
		return;
	}
#pragma endregion

	Enemy->OnAttackEnd.AddDynamic(this, &AKAEnemyAIController::OnAttackEndReceived);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AKAEnemyAIController::OnAttackEndReceived()
{
#pragma region NullChecks
	if (!BTComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::OnAttackEndReceived|BTComponent is nullptr"))
		return;
	}
	if (!BTNode)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::OnAttackEndReceived|BTNode is nullptr"))
		return;
	}
#pragma endregion

	BTComponent->OnTaskFinished(static_cast<const UBTTaskNode*>(BTNode), EBTNodeResult::Succeeded);
}

void AKAEnemyAIController::SetBTComponent(UBehaviorTreeComponent* NewBTComponent)
{
	BTComponent = NewBTComponent;
}

void AKAEnemyAIController::SetBTNode(UBTNode* NewBTNode)
{
	BTNode = NewBTNode;
}

void AKAEnemyAIController::SetFocusDirection(AActor* AttackTarget, AKAEnemy* Enemy)
{
#pragma region NullChecks
	if (!AttackTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::SetFocusDirection|AttackTarget is nullptr"))
		return;
	}
	if (!Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::SetFocusDirection|Enemy is nullptr"))
		return;
	}
#pragma endregion

	if (Enemy->GetIsAttacking())
	{
		return;
	}
	const FVector AttackTargetLocation{AttackTarget->GetActorLocation()};
	const FVector EnemyLocation{Enemy->GetActorLocation()};

	const FRotator LookAtRotation{UKismetMathLibrary::FindLookAtRotation(EnemyLocation, AttackTargetLocation)};
	const FVector LookAtRotationVector{LookAtRotation.Vector()};

	const FVector FocalPoint{EnemyLocation + FVector(LookAtRotationVector.X, 0.f, 0.f)};

	SetFocalPoint(FocalPoint);
}
