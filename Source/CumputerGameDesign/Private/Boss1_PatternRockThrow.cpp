#include "Boss1.h"

#include "MainCharacter.h"
#include "MainGameModeBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ABoss1::StartPatternRockThrowJumping()
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
	SetActorRotationSmooth(GetTargetDirectionWithoutZ().Rotation(), 20.0f);
	CanPatternRockThrow = false;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), RockThrowStartSound, GetActorLocation());
	GetWorldTimerManager().SetTimer(
		PatternTimer,
		[&]() -> void
		{
			if (PatternRockThrowNowRockCount >= PatternRockThrowTotalRockCount)
			{
				GetWorldTimerManager().ClearTimer(PatternTimer);
				StartPatternRockThrowLanding();
			}
			
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), RockThrowSound, GetActorLocation());
			PatternRockThrowNowRockCount++;
			const auto TempRock = GetWorld()->SpawnActor<ARock1>(Rock, GetActorLocation() + FVector(0, 0, 100), FRotator::ZeroRotator);
			const auto Direction = GetTargetDirection() - FVector(0, 0, 400);
			TempRock->Sphere->AddImpulse(Direction * 10000);
		},
		1.0f,
		true);
}

void ABoss1::StartPatternRockThrowLanding()
{
	State = EBossState::Casting;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	GetWorldTimerManager().SetTimer(
		PatternTimer,
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
			[&]() -> void { State = EBossState::Idle; },
			LandingDelay,
			false);
	}
}
