// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"

#include "Boss1.h"
#include "MainCharacter.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"

AMainGameModeBase::AMainGameModeBase()
{

	BgmComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BgmComp"));
	BgmComponent->SetupAttachment(RootComponent);

	Bgm1 = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sounds/Thunder-Unison-Action-Dramatic-Epic-Music-chosic_com_.Thunder-Unison-Action-Dramatic-Epic-Music-chosic_com_'"));
	Bgm2 = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sounds/Executioner_chosic_com_.Executioner_chosic_com_'"));
}

void AMainGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	SpawnBoss();
	SetBgm1();
}

void AMainGameModeBase::SpawnBoss()
{
	Boss = GetWorld()->SpawnActor<ABoss1>(FVector(0, 0, 1000), FRotator(0, 0, 0));
	//Boss = GetWorld()->SpawnActor<ABoss1>(FVector(1000, 1000, 1000), FRotator(0, 0, 0));
	//Boss->GetCharacterMovement()->Velocity = FVector(1000, 1000, 0);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), Boss->GetCharacterMovement()->Ph);
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