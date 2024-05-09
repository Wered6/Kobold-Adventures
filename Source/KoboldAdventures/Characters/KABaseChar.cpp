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
	CurrentHealth -= Damage;

	GetAnimInstance()->JumpToNode(TEXT("JumpHit"));

	if (CurrentHealth <= 0)
	{
		GetAnimInstance()->JumpToNode(TEXT("JumpDeath"));
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
