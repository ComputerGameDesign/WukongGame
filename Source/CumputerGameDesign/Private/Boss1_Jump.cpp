#include "Boss1.h"

#include "MainCharacter.h"
#include "MainGameModeBase.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ABoss1::StartJumping()
{
	SetCoolTime(CanJumpToPlayer, JumpCoolTime);
	State = EBossState::Casting;
	
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	
	const FVector Direction = GetTargetDirectionWithoutZ();
	SetActorRotationSmooth(Direction.Rotation(), 20.0f);

	JumpStartPosition = GetActorLocation();
	JumpTargetPosition = TargetPlayer->GetActorLocation();
	JumpTargetPosition.Z = JumpZLocation;
	JumpDeltaSeconds = 0;

	GetWorldTimerManager().SetTimer(
		PatternTimer,
		[&]() -> void
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), JumpingSound, GetActorLocation());
			GetCharacterMovement()->GravityScale = 0;
			State = EBossState::Jumping;
		},
		JumpCastingDelay,
		false);
}

void ABoss1::Jumping()
{
	constexpr float Delta = 0.1f;
	if (FMath::IsNearlyEqual(GetActorLocation().X, JumpTargetPosition.X, Delta) &&
		FMath::IsNearlyEqual(GetActorLocation().Y, JumpTargetPosition.Y, Delta) &&
		FMath::IsNearlyEqual(GetActorLocation().Z, JumpTargetPosition.Z, Delta))
	{
		StartLanding();
	}
	else
	{
		JumpDeltaSeconds += GetWorld()->GetDeltaSeconds();
		const FVector NewLocation = FMath::VInterpTo(JumpStartPosition, JumpTargetPosition, JumpDeltaSeconds, 2.0f);
		SetActorLocation(NewLocation);
	}
}

void ABoss1::StartLanding()
{
	State = EBossState::Casting;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	GetWorldTimerManager().SetTimer(
		PatternTimer,
		[&]() -> void
		{
			GetCharacterMovement()->GravityScale = 1.0f;
			LaunchCharacter(FVector(0, 0, -JumpLandingVelocity), true, true);
			State = EBossState::Landing;
		},
		JumpLandingDelay,
		false);
}


void ABoss1::Landing()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		UGameplayStatics::PlaySoundAtLocation(this, LandingSound, GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),  // 현재 월드
			LandingEffect,  // 사용할 파티클 시스템
			GetActorLocation(),
			FRotator(90, 0, 0),
			FVector(5, 5, 5)
		);
		
		auto Player = Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode())->Player;
		if (!Player->GetCharacterMovement()->IsFalling())
		{
			Player->TakeDamageToThis(JumpDamage);
		}

		State = EBossState::Casting;
		GetWorldTimerManager().SetTimer(
			PatternTimer,
			[&]() -> void { AttackMeleeOnce(); },
			LandingDelay,
			false);
	}
}
