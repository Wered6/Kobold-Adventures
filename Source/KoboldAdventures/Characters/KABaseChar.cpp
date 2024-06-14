// Fill out your copyright notice in the Description page of Project Settings.


#include "KABaseChar.h"
#include "PaperZDAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AKABaseChar::AKABaseChar()
{
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
}

void AKABaseChar::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void AKABaseChar::ReceiveDamage(const float Damage)
{
#pragma region NullChecks
	if (!HitAnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKABaseChar::ReceiveDamage|HitAnimSequence is nullptr"))
		return;
	}
#pragma endregion

	CurrentHealth -= Damage;

	if (CurrentHealth <= 0)
	{
		HandleDeath();
	}
}

bool AKABaseChar::GetIsAttacking() const
{
	return bIsAttacking;
}

void AKABaseChar::HandleDeath() const
{
	GetAnimInstance()->JumpToNode(TEXT("JumpDeath"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
