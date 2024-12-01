// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"

#include "Boss1.h"
#include "MainCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"

AMainGameModeBase::AMainGameModeBase()
{
	BgmComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BgmComp"));
	BgmComponent->SetupAttachment(RootComponent);

	Bgm1 = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sounds/Thunder-Unison-Action-Dramatic-Epic-Music-chosic_com_.Thunder-Unison-Action-Dramatic-Epic-Music-chosic_com_'"));
	Bgm2 = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sounds/Executioner_chosic_com_.Executioner_chosic_com_'"));
	ClearSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sounds/epic-cinematic-music-109252.epic-cinematic-music-109252'"));
	YouDiedSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sounds/tubular-bell-of-death-89485.tubular-bell-of-death-89485'"));
}

void AMainGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeCounts();
	
	Player = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);

	SpawnBoss();
	SetBgm1();

	StartTime = GetWorld()->GetTimeSeconds();
}

void AMainGameModeBase::PlayerDie()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		[&]() -> void
		{
			UGameplayStatics::PlaySound2D(this, YouDiedSound);
			UGameplayStatics::SetGamePaused(this, true);
			
			auto PlayerController = GetWorld()->GetFirstPlayerController();
			
			PlayerController->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			
			CreateWidget<UUserWidget>(PlayerController, FailWidget)->AddToViewport();
		},
		3.0f,
		false
	);
}

void AMainGameModeBase::BossDie()
{
	for (const auto Clone : BossClones)
	{
		if (Clone != nullptr)
		{
			Clone->Die();
		}
	}
	
	EndTime = GetWorld()->GetTimeSeconds();
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		[&]() -> void
		{
			UGameplayStatics::PlaySound2D(this, ClearSound);
			UGameplayStatics::SetGamePaused(this, true);
			
			auto PlayerController = GetWorld()->GetFirstPlayerController();
			PlayerController->bShowMouseCursor = true;

			// 마우스 입력 모드 설정 (UI와 상호작용하기 위해)
			FInputModeUIOnly InputMode;
			PlayerController->SetInputMode(InputMode);
		
			CreateWidget<UUserWidget>(PlayerController, SuccessWidget)->AddToViewport();
		},
		2.0f,
		false
	);
}

void AMainGameModeBase::SpawnBoss()
{
	Boss = GetWorld()->SpawnActor<ABoss1>(FVector(0, 0, 1000), FRotator(0, 0, 0));
}

void AMainGameModeBase::SetBgm1() const
{
	BgmComponent->SetSound(Bgm1);
	BgmComponent->Play();
}

void AMainGameModeBase::SetBgm2() const
{
	BgmComponent->SetSound(Bgm2);
	BgmComponent->Play();
}

void AMainGameModeBase::InitializeCounts()
{
	HitCount = 0;
	ShootCount = 0;
	RushCount = 0;
	JumpCount = 0;
}

FString AMainGameModeBase::GetClearTime() const
{
	int32 ClearTime = EndTime - StartTime;
	return FString::Printf(TEXT("%02d:%02d"), ClearTime / 60, ClearTime % 60);
}

FString AMainGameModeBase::GetHitRateString() const
{
	return FString::Printf(TEXT("%.2f%%"), 100.0f * HitCount / ShootCount);
}
