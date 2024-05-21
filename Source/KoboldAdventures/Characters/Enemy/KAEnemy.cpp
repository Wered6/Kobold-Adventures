// Fill out your copyright notice in the Description page of Project Settings.


#include "KAEnemy.h"
#include "PaperZDAnimInstance.h"

void AKAEnemy::Attack()
{
#pragma region NullChecks
	if (!AttackAnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemy::Attack|AttackAnimSequence is nullptr"))
		return;
	}
#pragma endregion

	bIsAttacking = true;

	// On end of animation call on attack end
	FZDOnAnimationOverrideEndSignature EndAnimDelegate;
	EndAnimDelegate.BindLambda([this](bool bResult)
	{
		// You can use bResult to differentiate between OnCompleted and OnCancelled
		OnAttackEnd.Broadcast();
		bIsAttacking = false;
	});

	GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, TEXT("DefaultSlot"), 1, 0, EndAnimDelegate);
}

AKAPatrolRoute* AKAEnemy::GetPatrolRoute_Implementation()
{
#pragma region NullChecks
	if (!PatrolRoute)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemy::GetPatrolRoute_Implementation|PatrolRoute is nullptr"))
		return nullptr;
	}
#pragma endregion

	return PatrolRoute;
}
