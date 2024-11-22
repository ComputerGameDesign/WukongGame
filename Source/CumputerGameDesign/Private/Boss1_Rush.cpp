#include "Boss1.h"

#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ABoss1::StartRushing()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), RushStartSound, GetActorLocation());
	SetCoolTime(CanRush, RushCoolTime);
	State = EBossState::RushTracing;
	GetWorldTimerManager().SetTimer(
		PatternTimer,
		[&]() -> void
		{
			State = EBossState::Rushing;
		},
		RushTracingTime,
		false);
}

void ABoss1::RushTracing()
{
	SetActorRotationSmoothOnce(GetTargetDirectionWithoutZ().Rotation(),20.0f, GetWorld()->DeltaTimeSeconds);
}

void ABoss1::Rushing()
{
	GetCharacterMovement()->Velocity = GetTransform().TransformVector(GetActorLocation().ForwardVector) * RushSpeed;
}
