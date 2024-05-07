// Fill out your copyright notice in the Description page of Project Settings.


#include "KAKobold.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PaperZDAnimInstance.h"

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

#pragma region EarlyReturn
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
#pragma region EarlyReturn
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

#pragma region EarlyReturn
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
	if (!bIsAttacking)
	{
		const float ValueX{Value.Get<float>()};

		AddMovementInput(FVector(1.f, 0.f, 0.f), ValueX);
		UpdateControllerRotation(ValueX);
	}
}

void AKAKobold::UpdateControllerRotation(const float DirectionX) const
{
#pragma region EarlyReturn
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
	Jump();
}

void AKAKobold::OnStopJump()
{
	StopJumping();
}

void AKAKobold::OnAttack()
{
#pragma region EarlyReturn
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

	if (!bIsAttacking)
	{
		if (AttackComboTimerHandle.IsValid())
		{
			// Clear timer earlier
			AttackComboTimerHandle.Invalidate();
		}

		bIsAttacking = true;

		switch (CurrentAttack)
		{
		case EAttackType::NONE:
			{
				// CurrentAttack can't be NONE
				break;
			}
		case EAttackType::ATTACK1:
			{
				PlayAttackAnimation(Attack1AnimSequence, EAttackType::ATTACK2);
				break;
			}
		case EAttackType::ATTACK2:
			{
				PlayAttackAnimation(Attack2AnimSequence, EAttackType::ATTACK3);
				break;
			}
		case EAttackType::ATTACK3:
			{
				PlayAttackAnimation(Attack3AnimSequence, EAttackType::ATTACK1);
				break;
			}
		}
		GetWorld()->GetTimerManager().SetTimer(AttackComboTimerHandle, this, &AKAKobold::ResetNextAttack,
		                                       AttackComboTime);
	}
}

void AKAKobold::ResetNextAttack()
{
	CurrentAttack = EAttackType::ATTACK1;
}

void AKAKobold::PlayAttackAnimation(const UPaperZDAnimSequence* AttackAnimSequence, EAttackType NextAttack)
{
#pragma region EarlyReturn
	if (!AttackAnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("AKAKobold::PlayAttackAnimation|AttackAnimSequence is nullptr"))
		return;
	}
#pragma endregion

	FZDOnAnimationOverrideEndSignature EndAnimDelegate;
	EndAnimDelegate.BindLambda([this, NextAttack](bool bResult)
	{
		// You can use bResult to differentiate between OnCompleted and OnCancelled
		bIsAttacking = false;
		CurrentAttack = NextAttack;
	});
	GetAnimInstance()->PlayAnimationOverride(AttackAnimSequence, TEXT("DefaultSlot"), 1.f, 0.f, EndAnimDelegate);
}
