// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss1Anim.h"
#include "Boss1.h"

void UBoss1Anim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (const ABoss1* Boss = Cast<ABoss1>(TryGetPawnOwner()))
	{
		IsFalling = Boss->State == EBossState::Jumping;
		IsGroggy = Boss->State == EBossState::Groggy;
		IsRushTracing = Boss->State == EBossState::RushTracing;
		IsRushing = Boss->State == EBossState::Rushing;
		IsAttack = Boss->State == EBossState::Attack;
		IsAttackMoving = Boss->State == EBossState::AttackMoving;
		if (IsAttack)
			AttackIndex = FMath::RandRange(0, AttackMaxIndex);
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
