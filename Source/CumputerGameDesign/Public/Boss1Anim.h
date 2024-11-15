// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Boss1Anim.generated.h"

/**
 * 
 */
UCLASS()
class CUMPUTERGAMEDESIGN_API UBoss1Anim : public UAnimInstance
{
	GENERATED_BODY()


public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsJumping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCasting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsGroggy = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRushTracing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRushing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttackMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackMaxIndex = 4;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsNeutralizing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCloning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDie = false;

	UFUNCTION(BlueprintCallable)
	bool IsJogFwd() const;

	UFUNCTION(BlueprintCallable)
	bool AttackIndexEqualWith(const int32 Index) const;

	UFUNCTION(BlueprintCallable)
	void EndAttack() const;
};
