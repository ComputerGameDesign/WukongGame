// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

#include "Boss1.h"
#include "MainCharacter.h"
#include "MainGameModeBase.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

class UImage;

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

}

float UMainUI::GetBossHpPercent() const
{
	const auto Gb = Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode());
	const float MaxHp = Gb->Boss->MaxHp;
	const float NowHp = Gb->Boss->Hp;

	return NowHp / MaxHp;
}

float UMainUI::GetBossShieldPercent() const
{
	const auto Gb = Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode());
	const float MaxShield = Gb->Boss->NowMaxShield;
	const float NowShield = Gb->Boss->Shield;

	return NowShield / MaxShield;
}

float UMainUI::GetPlayerHpPercent() const
{
	const auto Gb = Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode());
	const float MaxHp = Gb->Player->MaxHp;
	const float NowHp = Gb->Player->Hp;

	return NowHp / MaxHp;
}

FString UMainUI::GetBulletString() const
{
	const auto Gb = Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode());
	const int32 MaxBullet = Gb->Player->MaxBulletCount;
	const int32 NowBullet = Gb->Player->NowBulletCount;

	return FString::Printf(TEXT("%d/%d"), NowBullet, MaxBullet);
}


