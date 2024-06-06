// Fill out your copyright notice in the Description page of Project Settings.


#include "KAEnemy.h"
#include "PaperZDAnimInstance.h"
#include "AI/Controller/KAEnemyAIController.h"

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

void AKAEnemy::HandleDeath() const
{
	Super::HandleDeath();

	AKAEnemyAIController* KAEnemyAIC{Cast<AKAEnemyAIController>(GetController())};

#pragma region NullChecks
	if (!KAEnemyAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemy::HandleDeath|KAEnemyAIC is nullptr"))
		return;
	}
#pragma endregion

	KAEnemyAIC->SetStateAsDead();
	KAEnemyAIC->SetHasFocus(false);
}

AKAPatrolRoute* AKAEnemy::GetPatrolRoute_Implementation()
{
	return PatrolRoute;
}
