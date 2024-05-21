// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KAPatrolRoute.generated.h"

class USplineComponent;

UCLASS()
class KOBOLDADVENTURES_API AKAPatrolRoute : public AActor
{
	GENERATED_BODY()

public:
	AKAPatrolRoute();

	UFUNCTION(BlueprintCallable, Category="KA")
	UPARAM(DisplayName="WorldPosition")
	FVector GetSplinePointAsWorldPosition() const;
	UFUNCTION(BlueprintCallable, Category="KA")
	void IncrementPatrolRoute();

private:
	UPROPERTY(VisibleAnywhere, Category="KA")
	TObjectPtr<USplineComponent> PatrolRoute;

	UPROPERTY(VisibleAnywhere, Category="KA")
	uint8 PatrolIndex;
	UPROPERTY(VisibleAnywhere, Category="KA")
	int8 Direction{1};
};
