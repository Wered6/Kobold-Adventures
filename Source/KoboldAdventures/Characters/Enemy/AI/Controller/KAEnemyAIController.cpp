// Fill out your copyright notice in the Description page of Project Settings.


#include "KAEnemyAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "KoboldAdventures/Characters/Enemy/KAEnemy.h"

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
