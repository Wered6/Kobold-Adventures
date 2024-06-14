// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "KABaseChar.generated.h"

class UPaperZDAnimSequence;

UCLASS()
class KOBOLDADVENTURES_API AKABaseChar : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	AKABaseChar();

protected:
	virtual void BeginPlay() override;

#pragma region Combat

public:
	UFUNCTION(BlueprintCallable, Category="KA|Combat")
	virtual void ReceiveDamage(const float Damage);

	bool GetIsAttacking() const;

protected:
	virtual void HandleDeath() const;

	UPROPERTY(BlueprintReadOnly, Category="KA|Combat")
	bool bIsAttacking{false};
	// todo change it to enum

	UPROPERTY(EditDefaultsOnly, Category="KA|Combat")
	TObjectPtr<UPaperZDAnimSequence> HitAnimSequence;

#pragma endregion

#pragma region Health

private:
	UPROPERTY(VisibleAnywhere, Category="KA|Health")
	float MaxHealth{100.f};
	UPROPERTY(VisibleAnywhere, Category="KA|Health")
	float CurrentHealth;
#pragma endregion
};
