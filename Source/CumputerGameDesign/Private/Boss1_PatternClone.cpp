#include "Boss1.h"
#include "Boss1Clone.h"

#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABoss1::StartPatternCloneJumping()
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
			State = EBossState::PatternCloneJumping;
		},
		JumpCastingDelay,
		false);
}

void ABoss1::PatternCloneJumping()
{
	constexpr float Delta = 0.1f;
	if (FMath::IsNearlyEqual(GetActorLocation().X, JumpTargetPosition.X, Delta) &&
		FMath::IsNearlyEqual(GetActorLocation().Y, JumpTargetPosition.Y, Delta) &&
		FMath::IsNearlyEqual(GetActorLocation().Z, JumpTargetPosition.Z, Delta))
	{
		StartPatternCloneLanding();
	}
	else
	{
		JumpDeltaSeconds += GetWorld()->GetDeltaSeconds();
		const FVector NewLocation = FMath::VInterpTo(JumpStartPosition, JumpTargetPosition, JumpDeltaSeconds, 2.0f);
		SetActorLocation(NewLocation);
	}
}

void ABoss1::StartPatternCloneLanding()
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
			State = EBossState::PatternCloneLanding;
		},
		JumpLandingDelay,
		false);
}

void ABoss1::PatternCloneLanding()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		State = EBossState::Casting;
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			[&]() -> void { StartPatternClone(); },
			LandingDelay,
			false);
	}
}

void ABoss1::StartPatternClone()
{
	CanPatternClone = false;

	for (int32 i = 0; i < PatternCloneCount; i++)
	{
		if (auto c= GetWorld()->SpawnActor<ABoss1Clone>(Clone, GetActorLocation(), GetActorRotation()))
		{
			c->PersistentTime = PatternClonePersistentTime;
			c->SetMove(PatternClonePositions[i]);
		}
	}
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		[&]() -> void { State = EBossState::Idle; },
		PatternClonePersistentTime,
		false);
}

