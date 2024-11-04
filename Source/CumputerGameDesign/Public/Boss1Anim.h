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
	float Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsGroggy = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRushTracing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackMaxIndex = 4;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackIndex = 0;

	UFUNCTION(BlueprintCallable)
	bool IsJogFwd() const;

	UFUNCTION(BlueprintCallable)
	bool AttackIndexEqualWith(const int32 Index) const;
};
