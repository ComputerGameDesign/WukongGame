// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss1Anim.h"
#include "Boss1.h"
#include "Animation/AnimExecutionContext.h"
#include "Animation/AnimNodeReference.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBoss1Anim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (const ABoss1* Boss = Cast<ABoss1>(TryGetPawnOwner()))
	{
		IsSpawning = Boss->State == EBossState::Spawn;
		
		IsJumping = Boss->State == EBossState::Jumping ||
					Boss->State == EBossState::PatternNeutralizeJumping ||
					Boss->State == EBossState::PatternRockThrowJumping ||
					Boss->State == EBossState::PatternCloneJumping ||
					Boss->State == EBossState::Landing ||
					Boss->State == EBossState::PatternNeutralizeLanding ||
					Boss->State == EBossState::PatternRockThrowLanding ||
					Boss->State == EBossState::PatternCloneLanding ||
					Boss->GetCharacterMovement()->IsFalling();

		IsCasting = Boss->State == EBossState::Casting;
		
		IsGroggy = Boss->State == EBossState::Groggy;
		IsRushTracing = Boss->State == EBossState::RushTracing;
		IsRushing = Boss->State == EBossState::Rushing;
		IsAttack = Boss->State == EBossState::Attack;
		IsAttackMoving = Boss->State == EBossState::AttackMoving;
		if (IsAttack)
			AttackIndex = FMath::RandRange(0, AttackMaxIndex);

		IsNeutralizing = Boss->State == EBossState::Neutralized;
		IsCloning = Boss->State == EBossState::PatternCloning;

		IsDie = Boss->State == EBossState::Die;
	}
}

bool UBoss1Anim::IsJogFwd() const
{
	return IsRushing || IsAttackMoving;
}

bool UBoss1Anim::AttackIndexEqualWith(const int32 Index) const
{
	return AttackIndex == Index;
}

void UBoss1Anim::EndAttack() const
{
	if (const ABoss1* Boss = Cast<ABoss1>(TryGetPawnOwner()))
	{
		Boss->State = EBossState::Idle;
	}
}
