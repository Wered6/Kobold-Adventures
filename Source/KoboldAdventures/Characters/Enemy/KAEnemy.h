// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/KAEnemyAIInterface.h"
#include "KoboldAdventures/Characters/KABaseChar.h"
#include "KAEnemy.generated.h"

class AKAPatrolRoute;

UENUM(BlueprintType)
enum class EKAAIState : uint8
{
	Passive,
	Attacking,
	Frozen,
	Dead
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class KOBOLDADVENTURES_API AKAEnemy : public AKABaseChar, public IKAEnemyAIInterface
{
	GENERATED_BODY()

#pragma region Combat

public:
	UFUNCTION(BlueprintCallable, Category="KA|Combat")
	void Attack();

	FOnAttackEnd OnAttackEnd;

private:
	UPROPERTY(EditDefaultsOnly, Category="KA|Combat")
	TObjectPtr<UPaperZDAnimSequence> AttackAnimSequence;

#pragma endregion

#pragma region Patrol

	UPROPERTY(EditInstanceOnly, Category="KA|AI|Patrol")
	TObjectPtr<AKAPatrolRoute> PatrolRoute;

#pragma endregion

#pragma region Interface

	// Overriding GetPatrolRoute from EnemyAIInterface
	virtual AKAPatrolRoute* GetPatrolRoute_Implementation() override;

#pragma endregion
};
