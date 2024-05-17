// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KAEnemyAIController.generated.h"

class UBTNode;
class UBehaviorTreeComponent;
class AKAEnemy;

UCLASS()
class KOBOLDADVENTURES_API AKAEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;

#pragma region Combat

public:
	UFUNCTION()
	void OnAttackEndReceived();
	void SetBTComponent(UBehaviorTreeComponent* NewBTComponent);
	void SetBTNode(UBTNode* NewBTNode);

private:
	UPROPERTY(VisibleAnywhere, Category="KA|Combat")
	TObjectPtr<UBehaviorTreeComponent> BTComponent;
	UPROPERTY(VisibleAnywhere, Category="KA|Combat")
	TObjectPtr<UBTNode> BTNode;

#pragma endregion

#pragma region Focus

public:
	void SetHasFocus(const bool bValue);
	bool GetHasFocus() const;

private:
	UFUNCTION(BlueprintCallable, Category="KA")
	void SetFocusDirection(AActor* AttackTarget, AKAEnemy* Enemy);

	UPROPERTY(VisibleAnywhere, Category="KA")
	bool bHasFocus{false};

#pragma endregion
};
