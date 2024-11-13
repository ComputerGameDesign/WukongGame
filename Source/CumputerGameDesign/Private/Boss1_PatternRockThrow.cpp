﻿#include "Boss1.h"

#include "MainCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABoss1::StartPatternRockThrowJumping()
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
			State = EBossState::PatternRockThrowJumping;
		},
		JumpCastingDelay,
		false);
}

void ABoss1::PatternRockThrowJumping()
{
	constexpr float Delta = 0.1f;
	if (FMath::IsNearlyEqual(GetActorLocation().X, JumpTargetPosition.X, Delta) &&
		FMath::IsNearlyEqual(GetActorLocation().Y, JumpTargetPosition.Y, Delta) &&
		FMath::IsNearlyEqual(GetActorLocation().Z, JumpTargetPosition.Z, Delta))
	{
		StartPatternRockThrow();
	}
	else
	{
		JumpDeltaSeconds += GetWorld()->GetDeltaSeconds();
		const FVector NewLocation = FMath::VInterpTo(JumpStartPosition, JumpTargetPosition, JumpDeltaSeconds, 2.0f);
		SetActorLocation(NewLocation);
	}
}

void ABoss1::StartPatternRockThrow()
{
	State = EBossState::Casting;
	CanPatternRockThrow = false;
	GetWorldTimerManager().SetTimer(
		PatternTimer,
		[&]() -> void
		{
			if (PatternRockThrowNowRockCount >= PatternRockThrowTotalRockCount)
			{
				GetWorldTimerManager().ClearTimer(PatternTimer);
				StartPatternRockThrowLanding();
			}
			
			PatternRockThrowNowRockCount++;
			auto TempRock = GetWorld()->SpawnActor<ARock1>(Rock, GetActorLocation() + FVector(0, 0, 100), FRotator::ZeroRotator);
			auto TargetLocation = GetTargetDirection();
			TempRock->Sphere->AddImpulse(TargetLocation * 10000);
		},
		1.0f,
		true);
}

void ABoss1::StartPatternRockThrowLanding()
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
			State = EBossState::PatternRockThrowLanding;
		},
		JumpLandingDelay,
		false);
}

void ABoss1::PatternRockThrowLanding()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		State = EBossState::Casting;
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			[&]() -> void { State = EBossState::Idle; },
			LandingDelay,
			false);
	}
}