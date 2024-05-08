// Fill out your copyright notice in the Description page of Project Settings.


#include "KABaseChar.h"
#include "PaperZDAnimInstance.h"
#include "Components/CapsuleComponent.h"

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
