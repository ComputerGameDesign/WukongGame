#include "Boss1.h"

#include "MainCharacter.h"
#include "Components/CapsuleComponent.h"

void ABoss1::StartAttack()
{
	SetCoolTime(CanAttack, AttackCoolTime);
	SetActorRotationSmooth(GetTargetDirectionWithoutZ().Rotation(), 20.0f);
	
	State = EBossState::Casting;

	AttackDeltaSeconds = 0;
	AttackStartPosition = GetActorLocation();
	AttackStartPosition.Z = 0;
	AttackTargetPosition = TargetPlayer->GetActorLocation();
	AttackTargetPosition.Z = 0;
	
	GetWorldTimerManager().SetTimer(
		PatternTimer,
		[&]() -> void
		{
			State = EBossState::AttackMoving;
		},
		AttackMovingCastingDelay,
		false);
}

void ABoss1::AttackMoving()
{
	constexpr float Delta = 0.1f;
	if (FMath::IsNearlyEqual(GetActorLocation().X, AttackTargetPosition.X, Delta) &&
		FMath::IsNearlyEqual(GetActorLocation().Y, AttackTargetPosition.Y, Delta))
	{
		State = EBossState::Casting;
		GetWorldTimerManager().SetTimer(
			PatternTimer,
			[&]() -> void
			{
				AttackMeleeOnce();
			},
			AttackCastingDelay,
			false);
	}
	else
	{
		AttackDeltaSeconds += GetWorld()->GetDeltaSeconds();
		FVector NewLocation = FMath::VInterpTo(AttackStartPosition, AttackTargetPosition, AttackDeltaSeconds, 5.0f);
		NewLocation.Z = GetActorLocation().Z;
		SetActorLocation(NewLocation);
	}
}

void ABoss1::AttackMeleeOnce()
{
	State = EBossState::Attack;
	SetActorRotationSmooth(GetTargetDirectionWithoutZ().Rotation(), 20.0f);
	
	GetWorldTimerManager().SetTimer(
		PatternTimer,
		[&]() -> void
		{
			State = EBossState::Idle;
		},
		AttackDelay,
		false);
}

void ABoss1::Attacking()
{
	const FVector BoneLocation1 = GetMesh()->GetBoneLocation(FName("root_weapon_r"));
	const FVector BoneLocation2 = GetMesh()->GetBoneLocation(FName("root_weapon_end_r"));
	FRotator BoneRotation = (BoneLocation2 - BoneLocation1).Rotation();
	BoneRotation.Pitch += 90;
	
	WeaponCollider->SetWorldLocation(BoneLocation1);
	WeaponCollider->SetWorldRotation(BoneRotation);
}
