// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KoboldAdventures/Characters/Enemy/KAEnemy.h"
#include "KAMushroom.generated.h"

UCLASS()
class KOBOLDADVENTURES_API AKAMushroom : public AKAEnemy
{
	GENERATED_BODY()

#pragma region Combat
	
public:
	UFUNCTION(BlueprintCallable, Category="KA|Combat")
	void SetDidHit(const bool bValue) const;

#pragma endregion 
};
