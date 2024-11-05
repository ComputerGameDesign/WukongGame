// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class CUMPUTERGAMEDESIGN_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	float GetBossHpPercent() const;

	UFUNCTION(BlueprintCallable)
	float GetBossShieldPercent() const;
	
	UFUNCTION(BlueprintCallable)
	float GetPlayerHpPercent() const;
	
	UFUNCTION(BlueprintCallable)
	FString GetBulletString() const;
};
