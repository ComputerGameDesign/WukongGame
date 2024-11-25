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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Boss)
	TArray<class ABoss1*> BossClones;

	UPROPERTY(EditAnywhere,	BlueprintReadWrite, Category="Bgm")
	class UAudioComponent* BgmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bgm")
	class USoundWave* Bgm1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bgm")
	class USoundWave* Bgm2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> FailWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> SuccessWidget;
	
	UFUNCTION()
	void PlayerDie();

	UFUNCTION()
	void BossDie();
	
	UFUNCTION()
	void SpawnBoss();

	UFUNCTION()
	void SetBgm1() const;

	UFUNCTION()
	void SetBgm2() const;

	UFUNCTION(BlueprintCallable)
	FString GetClearTime() const;

private:
	float StartTime;
	float EndTime;
};
