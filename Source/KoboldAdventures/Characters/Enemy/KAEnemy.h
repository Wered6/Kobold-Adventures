// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KoboldAdventures/Characters/KABaseChar.h"
#include "KAEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class KOBOLDADVENTURES_API AKAEnemy : public AKABaseChar
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
};
