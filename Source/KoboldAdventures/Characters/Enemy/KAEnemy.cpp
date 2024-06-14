// Fill out your copyright notice in the Description page of Project Settings.


#include "KAEnemy.h"
#include "PaperZDAnimInstance.h"
#include "AI/Controller/KAEnemyAIController.h"
#include "Components/BoxComponent.h"

AKAEnemy::AKAEnemy()
{
	AttackHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitBox"));
	AttackHitBox->SetCollisionProfileName(TEXT("AttackBox"));
	AttackHitBox->SetupAttachment(RootComponent);
	// todo consider if attackhitboxes should be in cpp or in bleuprints
	// todo rethink structure of player and enemy classes cpp or blueprints?
}

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

void AKAEnemy::SetAttackHitBoxCollision(const bool bSetActive)
{
#pragma region NullChecks
	if (!AttackHitBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemy::SetAttackHitBoxCollision|AttackHitBox is nullptr"))
		return;
	}
#pragma endregion

	const ECollisionEnabled::Type CollisionState{
		bSetActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision
	};

	AttackHitBox->SetCollisionEnabled(CollisionState);
}

void AKAEnemy::Stun()
{
#pragma region NullChecks
	if (!StunAnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemy::Stun|StunAnimSequence is nullptr"))
		return;
	}
#pragma endregion

	bIsAttacking = true;

	// On end of animation call on stun end
	FZDOnAnimationOverrideEndSignature EndAnimDelegate;
	EndAnimDelegate.BindLambda([this](bool bResult)
	{
		// You can use bResult to differentiate between OnCompleted and OnCancelled
		OnStunEnd.Broadcast();
		bIsAttacking = false;
	});

	GetAnimInstance()->PlayAnimationOverride(StunAnimSequence, TEXT("DefaultSlot"), 1, 0, EndAnimDelegate);
}

void AKAEnemy::ReceiveDamage(const float Damage)
{	
	GetAnimInstance()->PlayAnimationOverride(HitAnimSequence);
	
	Super::ReceiveDamage(Damage);
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
