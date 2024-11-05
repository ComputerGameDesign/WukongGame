// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CUMPUTERGAMEDESIGN_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGameModeBase();
	
protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Player)
	class AMainCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Boss)
	class ABoss1* Boss;

	UFUNCTION()
	void SpawnBoss();
};
