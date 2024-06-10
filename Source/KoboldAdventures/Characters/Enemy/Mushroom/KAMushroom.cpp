// Fill out your copyright notice in the Description page of Project Settings.


#include "KAMushroom.h"
#include "KoboldAdventures/Characters/Enemy/AI/Controller/KAEnemyAIController.h"

void AKAMushroom::SetDidHit(const bool bValue) const
{
	AKAEnemyAIController* KAEnemyAIC{Cast<AKAEnemyAIController>(GetController())};

#pragma region NullChecks
	if (!KAEnemyAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAMushroom::SetDidHit|KAEnemyAIC is nullptr"))
		return;
	}
#pragma endregion

	KAEnemyAIC->SetDidHit(bValue);
}
