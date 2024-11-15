// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnim.h"

#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	if (const AMainCharacter* Player = Cast<AMainCharacter>(TryGetPawnOwner()))
	{
		IsJumping = Player->GetCharacterMovement()->IsFalling();
		IsShooting = Player->IsShoot || Player->IsShootAll;
		IsReloading = Player->IsReloading;
		IsDie = Player->IsDie;
	}
}

bool UMainCharacterAnim::IsMoving() const
{
	return FMath::Abs(GetFront()) > 0.1f || FMath::Abs(GetRight()) > 0.1f;
}


float UMainCharacterAnim::GetFront() const
{
	if (const AMainCharacter* Player = Cast<AMainCharacter>(TryGetPawnOwner()))
	{
		return FVector::DotProduct(Player->GetCharacterMovement()->Velocity.GetSafeNormal(), Player->GetActorForwardVector());
	}
	return 0;
}

float UMainCharacterAnim::GetRight() const
{
	if (const AMainCharacter* Player = Cast<AMainCharacter>(TryGetPawnOwner()))
	{
		return FVector::DotProduct(Player->GetCharacterMovement()->Velocity.GetSafeNormal(), Player->GetActorRightVector());
	}
	return 0;
}