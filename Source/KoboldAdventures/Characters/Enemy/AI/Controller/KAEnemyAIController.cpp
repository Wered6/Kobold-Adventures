// Fill out your copyright notice in the Description page of Project Settings.


#include "KAEnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "KoboldAdventures/Characters/Enemy/KAEnemy.h"
#include "KoboldAdventures/Characters/Enemy/AI/KAPatrolRoute.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AKAEnemyAIController::AKAEnemyAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

#pragma region NullChecks
	if (!AIPerception)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::AKAEnemyAIController|AIPerception is nullptr"))
		return;
	}
	if (!SightConfig)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::AKAEnemyAIController|SightConfig is nullptr"))
		return;
	}
#pragma endregion

	SightConfig->SightRadius = 800.f;
	SightConfig->LoseSightRadius = 1200.f;
	SightConfig->PeripheralVisionAngleDegrees = 5.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(5.f);

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AKAEnemyAIController::OnPerceptionUpdated);
}

void AKAEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AKAEnemy* Enemy{Cast<AKAEnemy>(InPawn)};

#pragma region NullChecks
	if (!Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::OnPossess|Enemy is nullptr"))
		return;
	}
#pragma endregion

	Enemy->OnAttackEnd.AddDynamic(this, &AKAEnemyAIController::OnAttackEndReceived);
	Enemy->OnStunEnd.AddDynamic(this, &AKAEnemyAIController::OnStunEndReceived);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AKAEnemyAIController::OnAttackEndReceived()
{
#pragma region NullChecks
	if (!DefaultAttackBTComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::OnAttackEndReceived|BTComponent is nullptr"))
		return;
	}
	if (!DefaultAttackBTTaskNode)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::OnAttackEndReceived|BTNode is nullptr"))
		return;
	}
#pragma endregion

	DefaultAttackBTComponent->OnTaskFinished(DefaultAttackBTTaskNode, EBTNodeResult::Succeeded);
}

void AKAEnemyAIController::SetDefaultAttackBTComponent(UBehaviorTreeComponent* NewBTComponent)
{
#pragma region NullChecks
	if (!NewBTComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::SetDefaultAttackBTComponent|NewBTComponent is nullptr"))
		return;
	}
#pragma endregion

	DefaultAttackBTComponent = NewBTComponent;
}

void AKAEnemyAIController::SetDefaultAttackBTTaskNode(UBTTaskNode* NewBTNode)
{
#pragma region NullChecks
	if (!NewBTNode)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::SetDefaultAttackBTNode|NewBTNode is nullptr"))
		return;
	}
#pragma endregion

	DefaultAttackBTTaskNode = NewBTNode;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AKAEnemyAIController::OnStunEndReceived()
{
#pragma region NullChecks
	if (!StunBTComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::OnStunEndReceived|StunBTComponent is nullptr"))
		return;
	}
	if (!StunBTTaskNode)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::OnStunEndReceived|StunBTNode is nullptr"))
		return;
	}
#pragma endregion

	StunBTComponent->OnTaskFinished(StunBTTaskNode, EBTNodeResult::Succeeded);
}

void AKAEnemyAIController::SetStunBTComponent(UBehaviorTreeComponent* NewBTComponent)
{
#pragma region NullChecks
	if (!NewBTComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::SetStunBTComponent|NewBTComponent is nullptr"))
		return;
	}
#pragma endregion

	StunBTComponent = NewBTComponent;
}

void AKAEnemyAIController::SetStunBTTaskNode(UBTTaskNode* NewBTNode)
{
#pragma region NullChecks
	if (!NewBTNode)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::SetStunBTNode|NewBTNode is nullptr"))
		return;
	}
#pragma endregion

	StunBTTaskNode = NewBTNode;
}

void AKAEnemyAIController::SetDidHit(const bool bValue)
{
	bDidHit = bValue;
}

bool AKAEnemyAIController::GetDidHit() const
{
	return bDidHit;
}

void AKAEnemyAIController::SetHasFocus(const bool bValue)
{
	bHasFocus = bValue;
	ClearFocus(EAIFocusPriority::Gameplay);
}

bool AKAEnemyAIController::GetHasFocus() const
{
	return bHasFocus;
}

void AKAEnemyAIController::SetFocusDirection(AActor* AttackTarget, AKAEnemy* Enemy)
{
#pragma region NullChecks
	if (!AttackTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::SetFocusDirection|AttackTarget is nullptr"))
		return;
	}
	if (!Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::SetFocusDirection|Enemy is nullptr"))
		return;
	}
#pragma endregion

	const bool bIsAttacking{Enemy->GetIsAttacking()};
	if (bIsAttacking)
	{
		return;
	}

	if (bHasFocus)
	{
		const FVector AttackTargetLocation{AttackTarget->GetActorLocation()};
		const FVector EnemyLocation{Enemy->GetActorLocation()};

		const FRotator LookAtRotation{UKismetMathLibrary::FindLookAtRotation(EnemyLocation, AttackTargetLocation)};
		const FVector LookAtRotationVector{LookAtRotation.Vector()};

		const FVector FocalPoint{EnemyLocation + FVector(LookAtRotationVector.X, 0.f, 0.f)};

		SetFocalPoint(FocalPoint);
	}
}

bool AKAEnemyAIController::CanSenseActor(AActor* Actor, const EKAAISense Sense, FAIStimulus& StimulusOUT) const
{
#pragma region NullChecks
	if (!Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::CanSaveActor|Actor is nullptr"))
		return false;
	}
#pragma endregion

	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	AIPerception->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo);

	TArray StimulusArray{ActorPerceptionBlueprintInfo.LastSensedStimuli};
	for (FAIStimulus Stimulus : StimulusArray)
	{
		const TSubclassOf SenseClass{UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus)};
		switch (Sense)
		{
		case EKAAISense::None:
			break;
		case EKAAISense::Sight:
			{
				if (SenseClass->IsChildOf(UAISense_Sight::StaticClass()))
				{
					StimulusOUT = Stimulus;
					return Stimulus.WasSuccessfullySensed();
				}
				break;
			}
		}
	}
	return false;
}

void AKAEnemyAIController::HandleSensedSight(AActor* Actor)
{
#pragma region NullChecks
	if (!Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::HandleSensedSight|Actor is nullptr"))
		return;
	}
#pragma endregion

	const EKAAIState State{GetCurrentState()};
	const AActor* Player{UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)};

	switch (State)
	{
	case EKAAIState::Passive:
		{
			if (Player == Actor)
			{
				SetStateAsAttacking(Actor);
			}
			break;
		}
	case EKAAIState::Attacking:
		break;
	case EKAAIState::Stunned:
		break;
	case EKAAIState::Dead:
		break;
	}
}

void AKAEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		FAIStimulus Stimulus;
		const bool bCanSenseActor{CanSenseActor(Actor, EKAAISense::Sight, Stimulus)};
		if (bCanSenseActor)
		{
			HandleSensedSight(Actor);
		}
	}
}

void AKAEnemyAIController::MoveAlongPatrolRoute()
{
	APawn* AIPawn{GetPawn()};

#pragma region NullChecks
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::MoveAlongPatrolRoute|AIPawn is nullptr"))
		return;
	}
#pragma endregion

	const bool bImplementsInterface{AIPawn->Implements<UKAEnemyAIInterface>()};
	if (bImplementsInterface)
	{
		PatrolRoute = IKAEnemyAIInterface::Execute_GetPatrolRoute(AIPawn);
	}

#pragma region NullChecks
	if (!PatrolRoute)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::MoveAlongPatrolRoute|PatrolRoute is nullptr"))
		return;
	}
#pragma endregion

	const FVector GoalLocation{PatrolRoute->GetSplinePointAsWorldPosition()};

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(GoalLocation);
	MoveRequest.SetAcceptanceRadius(10.f);

	FNavPathSharedPtr NavPath;

	// Save RequestID to check in OnMoveCompleted, so it works only for those move requests
	const FPathFollowingRequestResult Result{MoveTo(MoveRequest, &NavPath)};
	CurrentMoveAlongPatrolRouteRequestID = Result.MoveId;
}

void AKAEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
#pragma region NullChecks
	if (!PatrolRoute)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::OnMoveCompleted|PatrolRoute is nullptr"))
		return;
	}
	if (!MoveAlongPatrolRouteBTComponent)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("AKAEnemyAIController::OnMoveCompleted|MoveAlongPatrolRouteBTComponent is nullptr"))
		return;
	}
#pragma endregion

	Super::OnMoveCompleted(RequestID, Result);

	// Check if completed move is MoveAlongPatrolRoute request and if success
	const bool bIsCurrentRequestID{RequestID == CurrentMoveAlongPatrolRouteRequestID};
	const bool bIsSuccess{Result.Code == EPathFollowingResult::Success};
	if (bIsCurrentRequestID && bIsSuccess)
	{
		PatrolRoute->IncrementPatrolRoute();
		MoveAlongPatrolRouteBTComponent->OnTaskFinished(MoveAlongPatrolRouteBTTaskNode, EBTNodeResult::Succeeded);
	}
}

void AKAEnemyAIController::SetMoveAlongPatrolRouteBTComponent(UBehaviorTreeComponent* NewBTComponent)
{
#pragma region NullChecks
	if (!NewBTComponent)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("AKAEnemyAIController::SetMoveAlongPatrolRouteBTComponent|NewBTComponent is nullptr"))
		return;
	}
#pragma endregion

	MoveAlongPatrolRouteBTComponent = NewBTComponent;
}

void AKAEnemyAIController::SetMoveAlongPatrolRouteBTTaskNode(UBTTaskNode* NewBTTaskNode)
{
#pragma region NullChecks
	if (!NewBTTaskNode)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAEnemyAIController::SetMoveAlongPatrolRouteBTNode|NewBTNode is nullptr"))
		return;
	}
#pragma endregion

	MoveAlongPatrolRouteBTTaskNode = NewBTTaskNode;
}
