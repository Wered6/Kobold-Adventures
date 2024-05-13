// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KoboldAdventures/Characters/KABaseChar.h"
#include "KAKobold.generated.h"

class UBoxComponent;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UPaperZDAnimSequence;

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	ATTACK1 UMETA(DisplayName="Attack1"),
	ATTACK2 UMETA(DisplayName="Attack2"),
	ATTACK3 UMETA(DisplayName="Attack3")
};

UCLASS()
class KOBOLDADVENTURES_API AKAKobold : public AKABaseChar
{
	GENERATED_BODY()

public:
	AKAKobold();

protected:
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

#pragma region Combat

public:
	UFUNCTION(BlueprintCallable, Category="KA|Combat")
	void SetAttackHitBoxCollision(const EAttackType AttackType, const bool bSetActive) const;

private:
	void OnAttack();
	void PlayAttackAnimation(const UPaperZDAnimSequence* AttackAnimSequence);
	void ResetCurrentAttack();

	UFUNCTION()
	void OnPlaybackSequenceCompleted(const UPaperZDAnimSequence* AnimSequence);

	UPROPERTY(EditDefaultsOnly, Category="KA|Combat")
	TObjectPtr<UPaperZDAnimSequence> Attack1AnimSequence;
	UPROPERTY(EditDefaultsOnly, Category="KA|Combat")
	TObjectPtr<UPaperZDAnimSequence> Attack2AnimSequence;
	UPROPERTY(EditDefaultsOnly, Category="KA|Combat")
	TObjectPtr<UPaperZDAnimSequence> Attack3AnimSequence;

	UPROPERTY(VisibleAnywhere, Category="KA|Combat")
	TObjectPtr<UBoxComponent> Attack1HitBox;
	UPROPERTY(VisibleAnywhere, Category="KA|Combat")
	TObjectPtr<UBoxComponent> Attack2HitBox;
	UPROPERTY(VisibleAnywhere, Category="KA|Combat")
	TObjectPtr<UBoxComponent> Attack3HitBox;
	
	UPROPERTY(VisibleAnywhere, Category="KA|Combat")
	bool bIsAttackQueued{false};
	UPROPERTY(VisibleAnywhere, Category="KA|Combat")
	EAttackType CurrentAttack{EAttackType::ATTACK1};
	UPROPERTY(VisibleAnywhere, Category="KA|Combat")
	FTimerHandle AttackComboTimerHandle;
	UPROPERTY(VisibleAnywhere, Category="KA|Combat")
	float NextComboAttackWindowTime{1.f};
#pragma endregion
};
