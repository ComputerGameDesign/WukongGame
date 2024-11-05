// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"

#include "Boss1.h"
#include "MainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AMainGameModeBase::AMainGameModeBase()
{
}

void AMainGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	SpawnBoss();
}

void AMainGameModeBase::SpawnBoss()
{
	Boss = GetWorld()->SpawnActor<ABoss1>(FVector(0, 0, 1000), FRotator(0, 0, 0));
	//Boss = GetWorld()->SpawnActor<ABoss1>(FVector(1000, 1000, 1000), FRotator(0, 0, 0));
	//Boss->GetCharacterMovement()->Velocity = FVector(1000, 1000, 0);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), Boss->GetCharacterMovement()->Ph);
}
