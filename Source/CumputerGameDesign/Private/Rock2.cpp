// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock2.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ARock2::ARock2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Sphere->SetRelativeScale3D(FVector(2.5, 2.5, 2.5));
	Damage = 30;

	BreakSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/small-rock-break-194553_Cue.small-rock-break-194553_Cue'"));
	BreakEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/ParagonSunWukong/FX/Particles/Wukong/Abilities/DoubleJump/FX/p_GroundSlam_Radius.p_GroundSlam_Radius'"));
}

// Called when the game starts or when spawned
void ARock2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARock2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARock2::OnFloorHit()
{
	Super::OnFloorHit();
	Destroy();
}


