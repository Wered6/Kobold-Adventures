// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "KABaseChar.generated.h"

UCLASS()
class KOBOLDADVENTURES_API AKABaseChar : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	AKABaseChar();
	
	virtual void BeginPlay() override;

#pragma region Combat

public:
	UFUNCTION(BlueprintCallable, Category="KA|Combat")
	void ReceiveDamage(const float Damage);

#pragma endregion 
	
#pragma region Health

private:
	UPROPERTY(VisibleAnywhere, Category="KA|Health")
	float MaxHealth{100.f};
	UPROPERTY(VisibleAnywhere, Category="KA|Health")
	float CurrentHealth;
#pragma endregion
};
