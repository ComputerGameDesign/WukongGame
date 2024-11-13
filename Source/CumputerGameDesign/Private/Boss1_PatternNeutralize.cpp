﻿#include "Boss1.h"

#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABoss1::StartPatternNeutralizeJumping()
{
	State = EBossState::Casting;
	JumpDeltaSeconds = 0;
	JumpStartPosition = GetActorLocation();
	JumpTargetPosition = FVector(0, 0, 1500);
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		[&]() -> void
		{
			GetCharacterMovement()->GravityScale = 0;
			State = EBossState::PatternNeutralizeJumping;
		},
		JumpCastingDelay,
		false);
}

void ABoss1::PatternNeutralizeJumping()
{
	constexpr float Delta = 0.1f;
	if (FMath::IsNearlyEqual(GetActorLocation().X, JumpTargetPosition.X, Delta) &&
		FMath::IsNearlyEqual(GetActorLocation().Y, JumpTargetPosition.Y, Delta) &&
		FMath::IsNearlyEqual(GetActorLocation().Z, JumpTargetPosition.Z, Delta))
	{
		StartPatternNeutralizeLanding();
	}
	else
	{
		JumpDeltaSeconds += GetWorld()->GetDeltaSeconds();
		const FVector NewLocation = FMath::VInterpTo(JumpStartPosition, JumpTargetPosition, JumpDeltaSeconds, 2.0f);
		SetActorLocation(NewLocation);
	}
}

void ABoss1::StartPatternNeutralizeLanding()
{
	State = EBossState::Casting;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		[&]() -> void
		{
			GetCharacterMovement()->GravityScale = 1.0f;
			LaunchCharacter(FVector(0, 0, -JumpLandingVelocity), true, true);
			State = EBossState::PatternNeutralizeLanding;
		},
		JumpLandingDelay,
		false);
}

void ABoss1::PatternNeutralizeLanding()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		State = EBossState::Casting;
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			[&]() -> void { StartPatternNeutralize(); },
			LandingDelay,
			false);
	}
}

void ABoss1::StartPatternNeutralize()
{
	State = EBossState::Neutralized;
	Shield = PatternNeutralizeShields[NowPatternNeutralizeCount];
	NowMaxShield = Shield;
	GetWorldTimerManager().SetTimer(
			PatternTimer,
			[&]() -> void
			{
				NowPatternNeutralizeCount++;
				Hp += Shield;
				Shield = 0;
				TargetPlayer->TakeDamage(PatternNeutralizeFailDamage);
				State = EBossState::Idle;
			},
			PatternNeutralizePersistentTime,
			false);
}

void ABoss1::PatternNeutralize()
{
	if (Shield <= 0)
	{
		GetWorldTimerManager().ClearTimer(PatternTimer);
		NowPatternNeutralizeCount++;
		State = EBossState::Groggy;
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(
			Timer,
			[&]() -> void
			{
				State = EBossState::Idle;
			},
			PatternNeutralizeSuccessGroggyTime,
			false);
	}
}