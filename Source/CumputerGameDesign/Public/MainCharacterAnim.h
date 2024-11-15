// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharacterAnim.generated.h"

/**
 * 
 */
UCLASS()
class CUMPUTERGAMEDESIGN_API UMainCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsJumping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsShooting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReloading = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDie = false;
	
	UFUNCTION(BlueprintCallable)
	bool IsMoving() const;
	
	UFUNCTION(BlueprintCallable)
	float GetFront() const;
	
	UFUNCTION(BlueprintCallable)
	float GetRight() const;
};
