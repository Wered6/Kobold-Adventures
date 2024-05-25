// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KAEnemyAIController.generated.h"

class UAISenseConfig_Sight;
struct FAIStimulus;
enum class EKAAIState : uint8;
class UBTNode;
class UBehaviorTreeComponent;
class AKAEnemy;

UENUM(BlueprintType)
enum class EKAAISense : uint8
{
	None,
	Sight
};

UCLASS()
class KOBOLDADVENTURES_API AKAEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AKAEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

#pragma region Combat

public:
	UFUNCTION()
	void OnAttackEndReceived();
	void SetDefaultAttackBTComponent(UBehaviorTreeComponent* NewBTComponent);
	void SetDefaultAttackBTNode(UBTNode* NewBTNode);

private:
	UPROPERTY(VisibleAnywhere, Category="KA|Combat")
	TObjectPtr<UBehaviorTreeComponent> DefaultAttackBTComponent;
	UPROPERTY(VisibleAnywhere, Category="KA|Combat")
	TObjectPtr<UBTNode> DefaultAttackBTNode;

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

#pragma region State

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="KA|State")
	void SetStateAsAttacking(AActor* AttackTarget);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="KA|State")
	void SetStateAsPassive();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="KA|State")
	UPARAM(DisplayName="State")
	EKAAIState GetCurrentState();

#pragma endregion

#pragma region Perception

private:
	bool CanSenseActor(AActor* Actor, EKAAISense Sense, FAIStimulus& StimulusOUT) const;
	void HandleSensedSight(AActor* Actor);
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UPROPERTY(VisibleAnywhere, Category="KA|Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerception;
	UPROPERTY(VisibleAnywhere, Category="KA|Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

#pragma endregion
};
