#include "Boss1.h"
#include "Boss1Clone.h"

#include "MainCharacter.h"
#include "MainGameModeBase.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ABoss1::StartPatternCloneJumping()
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
	GetWorldTimerManager().SetTimer(
		PatternTimer,
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
		GetWorldTimerManager().SetTimer(
			PatternTimer,
			[&]() -> void { StartPatternClone(); },
			LandingDelay,
			false);
	}
}

void ABoss1::StartPatternClone()
{
	State = EBossState::PatternCloning;
	CanPatternClone = false;
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), CloningSound, GetActorLocation());
	
	for (int32 i = 0; i < PatternCloneCount; i++)
	{
		if (auto c= GetWorld()->SpawnActor<ABoss1Clone>(Clone, GetActorLocation(), FRotationMatrix::MakeFromX(-PatternClonePositions[i]).Rotator()))
		{
			c->PersistentTime = PatternClonePersistentTime;
			c->SetMove(PatternClonePositions[i]);
		}
	}

	Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode())->SetBgm2();
	GetWorldTimerManager().SetTimer(
		PatternTimer,
		[&]() -> void { State = EBossState::Idle; },
		PatternClonePersistentTime,
		false);
}

