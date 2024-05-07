// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "KAKobold.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UPaperZDAnimSequence;

UENUM()
enum class EAttackType : uint8
{
	NONE UMETA(DisplayName="None"),
	ATTACK1 UMETA(DisplayName="Attack1"),
	ATTACK2 UMETA(DisplayName="Attack2"),
	ATTACK3 UMETA(DisplayerName="Attack3")
};

UCLASS()
class KOBOLDADVENTURES_API AKAKobold : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	void InitializePlayerController();

	UPROPERTY()
	TObjectPtr<APlayerController> PC;

#pragma region EnhancedInput

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void SetupInputMappingContext() const;

	UPROPERTY(EditDefaultsOnly, Category="KA|Enhanced Input")
	TObjectPtr<UInputMappingContext> IMC;
	UPROPERTY(EditDefaultsOnly, Category="KA|Enhanced Input")
	TObjectPtr<UInputAction> IA_Attack;
	UPROPERTY(EditDefaultsOnly, Category="KA|Enhanced Input")
	TObjectPtr<UInputAction> IA_Jump;
	UPROPERTY(EditDefaultsOnly, Category="KA|Enhanced Input")
	TObjectPtr<UInputAction> IA_Move;
#pragma endregion

#pragma region Movement

private:
	void OnMove(const FInputActionValue& Value);

	void UpdateControllerRotation(const float DirectionX) const;
#pragma endregion

#pragma region Jump

private:
	void OnStartJump();
	void OnStopJump();
#pragma endregion

#pragma region Attack

private:
	void OnAttack();
	void ResetNextAttack();
	void PlayAttackAnimation(const UPaperZDAnimSequence* AttackAnimSequence, EAttackType NextAttack);

	UPROPERTY(EditDefaultsOnly, Category="KA|Attack")
	TObjectPtr<UPaperZDAnimSequence> Attack1AnimSequence;
	UPROPERTY(EditDefaultsOnly, Category="KA|Attack")
	TObjectPtr<UPaperZDAnimSequence> Attack2AnimSequence;
	UPROPERTY(EditDefaultsOnly, Category="KA|Attack")
	TObjectPtr<UPaperZDAnimSequence> Attack3AnimSequence;

	UPROPERTY(VisibleAnywhere, Category="KA|Attack")
	bool bIsAttacking{false};
	UPROPERTY(VisibleAnywhere, Category="KA|Attack")
	EAttackType CurrentAttack{EAttackType::ATTACK1};
	UPROPERTY(VisibleAnywhere, Category="KA|Attack")
	FTimerHandle AttackComboTimerHandle;
	UPROPERTY(VisibleAnywhere, Category="KA|Attack")
	float AttackComboTime{1.f};
#pragma endregion
};
