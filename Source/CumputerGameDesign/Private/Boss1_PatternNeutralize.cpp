#include "Boss1.h"

#include "MainCharacter.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ABoss1::StartPatternNeutralizeJumping()
{
	State = EBossState::Casting;
	JumpDeltaSeconds = 0;
	JumpStartPosition = GetActorLocation();
	JumpTargetPosition = FVector(0, 0, 1500);
	
	GetWorldTimerManager().SetTimer(
		PatternTimer,
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
	GetWorldTimerManager().SetTimer(
		PatternTimer,
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
		SetActorRotationSmooth(GetTargetDirectionWithoutZ().Rotation(), 20.0f);
		GetWorldTimerManager().SetTimer(
			PatternTimer,
			[&]() -> void { StartPatternNeutralize(); },
			LandingDelay,
			false);
	}
}

void ABoss1::StartPatternNeutralize()
{
	State = EBossState::Neutralized;
	Shield = PatternNeutralizeShields[NowPatternNeutralizeCount];
	NowPatternNeutralizeCount++;
	NowMaxShield = Shield;
	NeutralizeAudioComp->Play();
	GetWorldTimerManager().SetTimer(
			PatternTimer,
			[&]() -> void
			{
				Hp += Shield;
				Shield = 0;
				NeutralizeAudioComp->Stop();
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NeutralizeFailEffect, GetActorLocation());
				TargetPlayer->TakeDamageToThis(PatternNeutralizeFailDamage);
				State = EBossState::Idle;
			},
			PatternNeutralizePersistentTime,
			false);
}

void ABoss1::PatternNeutralize()
{
	if (Shield <= 0)
	{
		NeutralizeAudioComp->Stop();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), NeutralizeSuccessSound, GetActorLocation());
		GetWorldTimerManager().ClearTimer(PatternTimer);
		State = EBossState::Groggy;
		GetWorldTimerManager().SetTimer(
			PatternTimer,
			[&]() -> void
			{
				State = EBossState::Idle;
			},
			PatternNeutralizeSuccessGroggyTime,
			false);
	}
}
