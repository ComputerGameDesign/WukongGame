// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock1.h"

#include "Rock2.h"
#include "Components/SphereComponent.h"
#include "Sound//SoundCue.h"

// Sets default values
ARock1::ARock1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Sphere->SetRelativeScale3D(FVector(5, 5, 5));
	Damage = 50;
	MiniRock = ARock2::StaticClass();

	BreakSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/rock-break-hard-184891_Cue.rock-break-hard-184891_Cue'"));
}

// Called when the game starts or when spawned
void ARock1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARock1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARock1::OnFloorHit()
{
	auto MiniRock1 = GetWorld()->SpawnActor<ARock2>(MiniRock, GetActorLocation() + FVector(50, 50, 100), GetActorRotation());
	auto MiniRock2 = GetWorld()->SpawnActor<ARock2>(MiniRock, GetActorLocation() + FVector(-50, -50, 100), GetActorRotation());

	
	MiniRock1->Sphere->AddImpulse(FVector(100000, 100000, 1500000));
	MiniRock2->Sphere->AddImpulse(FVector(-100000, -100000, 1500000));
	
	Destroy();
}


