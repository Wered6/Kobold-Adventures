// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/KAEnemyAIInterface.h"
#include "KoboldAdventures/Characters/KABaseChar.h"
#include "KAEnemy.generated.h"

class AKAEnemyAIController;
class UBoxComponent;
class AKAPatrolRoute;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStunEnd);

UCLASS()
class KOBOLDADVENTURES_API AKAEnemy : public AKABaseChar, public IKAEnemyAIInterface
{
	GENERATED_BODY()

public:
	AKAEnemy();

protected:
	virtual void BeginPlay() override;

#pragma region Combat

public:
	UFUNCTION(BlueprintCallable, Category="KA|Combat")
	void Attack() const;
	UFUNCTION(BlueprintCallable, Category="KA|Combat")
	void SetAttackHitBoxCollision(const bool bSetActive);

	UFUNCTION(BlueprintCallable, Category="KA|Combat")
	void Stun() const;

	virtual void ReceiveDamage(const float Damage) override;

	FOnStunEnd OnStunEnd;

	FOnAttackEnd OnAttackEnd;

private:
	virtual void HandleDeath() const override;

	UPROPERTY(EditDefaultsOnly, Category="KA|Combat")
	TObjectPtr<UPaperZDAnimSequence> AttackAnimSequence;

	UPROPERTY(EditDefaultsOnly, Category="KA|Combat")
	TObjectPtr<UPaperZDAnimSequence> StunAnimSequence;

	UPROPERTY(VisibleAnywhere, Category="KA|Combat")
	TObjectPtr<UBoxComponent> AttackHitBox;

	/*
	 *todo when entering in combat mode it stops for a second, fix it to be smooth (or not, think about it)
	 *maybe add some bubble text of exclamation mark or something
	*/

#pragma endregion

#pragma region Patrol

	UPROPERTY(EditInstanceOnly, Category="KA|AI|Patrol")
	TObjectPtr<AKAPatrolRoute> PatrolRoute;

#pragma endregion

#pragma region Interface

	// Overriding GetPatrolRoute from EnemyAIInterface
	virtual AKAPatrolRoute* GetPatrolRoute_Implementation() override;

#pragma endregion

protected:
	UPROPERTY()
	TObjectPtr<AKAEnemyAIController> KAEnemyAIC;
};
