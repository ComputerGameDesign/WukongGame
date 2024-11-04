// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss1Anim.h"
#include "Boss1.h"

void UBoss1Anim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (const ABoss1* Boss = Cast<ABoss1>(TryGetPawnOwner()))
	{
		const FVector Velocity = Boss->GetVelocity();
		const FVector ForwardVector = Boss->GetActorForwardVector();
		Speed = FVector::DotProduct(Velocity, ForwardVector);
		IsFalling = Boss->State == EBossState::Jumping;
		IsGroggy = Boss->State == EBossState::Groggy;
		IsRushTracing = Boss->State == EBossState::RushTracing;
		IsAttack = Boss->State == EBossState::Attack;
		if (IsAttack)
			AttackIndex = FMath::RandRange(0, AttackMaxIndex);
	}
}

bool UBoss1Anim::IsJogFwd() const
{
	return Speed > 0.1f;
}

bool UBoss1Anim::AttackIndexEqualWith(const int32 Index) const
{
	return AttackIndex == Index;
}
