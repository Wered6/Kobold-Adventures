// Fill out your copyright notice in the Description page of Project Settings.


#include "KAKobold.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PaperZDAnimInstance.h"
#include "AnimSequences/Players/PaperZDAnimPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AKAKobold::AKAKobold()
{
	Attack1HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack1HitBox"));
	Attack1HitBox->SetRelativeLocation(FVector(88.f, 0.f, -14.f));
	Attack1HitBox->SetBoxExtent(FVector(132.f, 32.f, 42.f));
	Attack1HitBox->SetCollisionProfileName(TEXT("AttackBox"));
	Attack1HitBox->SetupAttachment(RootComponent);

	Attack2HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack2HitBox"));
	Attack2HitBox->SetRelativeLocation(FVector(20.f, 0.f, -36.f));
	Attack2HitBox->SetBoxExtent(FVector(176.f, 32.f, 48.f));
	Attack2HitBox->SetCollisionProfileName(TEXT("AttackBox"));
	Attack2HitBox->SetupAttachment(RootComponent);

	Attack3HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack3HitBox"));
	Attack3HitBox->SetRelativeLocation(FVector(168.f, 0.f, 36.f));
	Attack3HitBox->SetBoxExtent(FVector(64.f, 32.f, 144.f));
	Attack3HitBox->SetCollisionProfileName(TEXT("AttackBox"));
	Attack3HitBox->SetupAttachment(RootComponent);
}

void AKAKobold::BeginPlay()
{
	Super::BeginPlay();

	InitializePlayerController();
	SetupInputMappingContext();
}

void AKAKobold::InitializePlayerController()
{
	PC = Cast<APlayerController>(GetController());
}

void AKAKobold::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent{Cast<UEnhancedInputComponent>(PlayerInputComponent)};

#pragma region NullChecks
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::SetupPlayerInputComponent|EnhancedInputComponent is nullptr"))
		return;
	}
	if (!IA_Move)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::SetupPlayerInputComponent|IA_Move is nullptr"))
		return;
	}
	if (!IA_Jump)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::SetupPlayerInputComponent|IA_Jump is nullptr"))
		return;
	}
	if (!IA_Attack)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::SetupPlayerInputComponent|IA_Attack is nullptr"))
		return;
	}
#pragma endregion

	EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AKAKobold::OnMove);
	EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AKAKobold::OnStartJump);
	EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AKAKobold::OnStopJump);
	EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &AKAKobold::OnAttack);
}

void AKAKobold::SetupInputMappingContext() const
{
#pragma region NullChecks
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::SetupInputMappingContext|PC is nullptr"))
		return;
	}
	if (!IMC)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::SetupInputMappingContext|IMC is nullptr"))
		return;
	}
#pragma endregion

	const ULocalPlayer* LocalPlayer{PC->GetLocalPlayer()};
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem{
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()
	};

#pragma region NullChecks
	if (!EnhancedInputLocalPlayerSubsystem)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("AKAKobold::SetupInputMappingContext|EnhancedInputLocalPlayerSubsystem is nullptr"))
		return;
	}
#pragma endregion

	EnhancedInputLocalPlayerSubsystem->AddMappingContext(IMC, 0);
}

void AKAKobold::OnMove(const FInputActionValue& Value)
{
	switch (CurrentState)
	{
	case EKAPlayerState::Passive:
		{
			const float ValueX{Value.Get<float>()};

			AddMovementInput(FVector(1.f, 0.f, 0.f), ValueX);
			UpdateControllerRotation(ValueX);
			break;
		}
	case EKAPlayerState::Attacking:
	case EKAPlayerState::Stunned:
	case EKAPlayerState::Dead:
		break;
	}
}

void AKAKobold::UpdateControllerRotation(const float DirectionX) const
{
#pragma region NullChecks
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::UpdateControllerRotation|PC is nullptr"))
		return;
	}
#pragma endregion

	const FRotator ControllerRotation{PC->GetControlRotation()};
	const FRotator RightDirection{FRotator::ZeroRotator};
	const FRotator LeftDirection{FRotator(0.f, 180.f, 0.f)};

	if (DirectionX > 0 && !ControllerRotation.Equals(RightDirection))
	{
		PC->SetControlRotation(RightDirection);
	}
	if (DirectionX < 0 && !ControllerRotation.Equals(LeftDirection))
	{
		PC->SetControlRotation(LeftDirection);
	}
}

void AKAKobold::OnStartJump()
{
	switch (CurrentState)
	{
	case EKAPlayerState::Passive:
		{
			Jump();
			break;
		}
	case EKAPlayerState::Attacking:
	case EKAPlayerState::Stunned:
	case EKAPlayerState::Dead:
		break;
	}
}

void AKAKobold::OnStopJump()
{
	StopJumping();
}

void AKAKobold::SetAttackHitBoxCollision(const EAttackType AttackType, const bool bSetActive) const
{
#pragma region NullChecks
	if (!Attack1HitBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::SetAttackHitBoxCollision|Attack1HitBox is nullptr"))
		return;
	}
	if (!Attack1HitBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::SetAttackHitBoxCollision|Attack1HitBox is nullptr"))
		return;
	}
	if (!Attack1HitBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::SetAttackHitBoxCollision|Attack1HitBox is nullptr"))
		return;
	}
#pragma endregion

	const ECollisionEnabled::Type CollisionState{
		bSetActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision
	};

	switch (AttackType)
	{
	case EAttackType::ATTACK1:
		Attack1HitBox->SetCollisionEnabled(CollisionState);
		break;
	case EAttackType::ATTACK2:
		Attack2HitBox->SetCollisionEnabled(CollisionState);
		break;
	case EAttackType::ATTACK3:
		Attack3HitBox->SetCollisionEnabled(CollisionState);
		break;
	}
}

void AKAKobold::ReceiveDamage(const float Damage)
{
	CurrentState = EKAPlayerState::Stunned;

	// On end of animation set CurrentState to passive
	FZDOnAnimationOverrideEndSignature EndAnimDelegate;
	EndAnimDelegate.BindLambda([this](bool bResult)
	{
		// You can use bResult to differentiate between OnCompleted and OnCancelled
		CurrentState = EKAPlayerState::Passive;
	});

	GetAnimInstance()->PlayAnimationOverride(HitAnimSequence, TEXT("DefaultSlot"), 1.f, 0.f, EndAnimDelegate);

	Super::ReceiveDamage(Damage);
}

void AKAKobold::OnAttack()
{
#pragma region NullChecks
	if (!Attack1AnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::OnAttack|Attack1AnimSequence is nullptr"))
		return;
	}
	if (!Attack2AnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::OnAttack|Attack2AnimSequence is nullptr"))
		return;
	}
	if (!Attack3AnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::OnAttack|Attack3AnimSequence is nullptr"))
		return;
	}
#pragma endregion

	const bool bIsInTheAir{!GetCharacterMovement()->IsMovingOnGround()};
	if (bIsInTheAir)
	{
		return;
	}

	// If we're attacking and no attack is queued we can queue next attack
	if (CurrentState == EKAPlayerState::Attacking && !bIsAttackQueued)
	{
		bIsAttackQueued = true;

		// Queueing next attack on end of current attack animation
		GetAnimInstance()->GetPlayer()->OnPlaybackSequenceComplete.AddDynamic(
			this, &AKAKobold::OnPlaybackSequenceCompleted);
	}
	// If we're passive we have NextComboAttackWindowTime to do next attack combo
	else if (CurrentState == EKAPlayerState::Passive)
	{
		// Clear timer, so it won't reset current attack
		FTimerManager& TimerManager{GetWorld()->GetTimerManager()};
		if (TimerManager.IsTimerActive(AttackComboTimerHandle))
		{
			TimerManager.ClearTimer(AttackComboTimerHandle);
		}

		// Play proper attack animation
		switch (CurrentAttack)
		{
		case EAttackType::ATTACK1:
			PlayAttackAnimation(Attack1AnimSequence);
			break;
		case EAttackType::ATTACK2:
			PlayAttackAnimation(Attack2AnimSequence);
			break;
		case EAttackType::ATTACK3:
			PlayAttackAnimation(Attack3AnimSequence);
			break;
		}
	}
}

void AKAKobold::PlayAttackAnimation(const UPaperZDAnimSequence* AttackAnimSequence)
{
#pragma region NullChecks
	if (!AttackAnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::PlayAttackAnimation|AttackAnimSequence is nullptr"))
		return;
	}
#pragma endregion

	CurrentState = EKAPlayerState::Attacking;

	switch (CurrentAttack)
	{
	case EAttackType::ATTACK1:
		CurrentAttack = EAttackType::ATTACK2;
		break;
	case EAttackType::ATTACK2:
		CurrentAttack = EAttackType::ATTACK3;
		break;
	case EAttackType::ATTACK3:
		CurrentAttack = EAttackType::ATTACK1;
		break;
	}

	// On end of animation set CurrentState to passive
	FZDOnAnimationOverrideEndSignature EndAnimDelegate;
	EndAnimDelegate.BindLambda([this](bool bResult)
	{
		// You can use bResult to differentiate between OnCompleted and OnCancelled
		if (bResult)
		{
			// If attack animation is completed we switch back to passive
			CurrentState = EKAPlayerState::Passive;
		}
		else
		{
			// If attack animation in cancelled its probably because of getting hit and hit animation override this
			CurrentState = EKAPlayerState::Stunned;
		}
	});
	GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, TEXT("DefaultSlot"), 1.f, 0.f, EndAnimDelegate);

	// Set time window for attack combo
	GetWorld()->GetTimerManager().SetTimer(AttackComboTimerHandle, this, &AKAKobold::ResetCurrentAttack,
	                                       NextComboAttackWindowTime);
}

void AKAKobold::ResetCurrentAttack()
{
	CurrentAttack = EAttackType::ATTACK1;
}

void AKAKobold::OnPlaybackSequenceCompleted(const UPaperZDAnimSequence* AnimSequence)
{
#pragma region NullChecks
	if (!AnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::OnPlaybackSequenceCompleted|AnimSequence is nullptr"))
		return;
	}
	if (!Attack1AnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::OnPlaybackSequenceCompleted|Attack1AnimSequence is nullptr"))
		return;
	}
	if (!Attack2AnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::OnPlaybackSequenceCompleted|Attack2AnimSequence is nullptr"))
		return;
	}
	if (!Attack3AnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::OnPlaybackSequenceCompleted|Attack3AnimSequence is nullptr"))
		return;
	}
#pragma endregion

	bIsAttackQueued = false;

	if (AnimSequence == Attack1AnimSequence)
	{
		PlayAttackAnimation(Attack2AnimSequence);
	}
	else if (AnimSequence == Attack2AnimSequence)
	{
		PlayAttackAnimation(Attack3AnimSequence);
	}

	GetAnimInstance()->GetPlayer()->OnPlaybackSequenceComplete.RemoveAll(this);
}
