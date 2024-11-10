// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock1.h"

#include "Rock2.h"
#include "Components/SphereComponent.h"

// Sets default values
ARock1::ARock1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Sphere->SetRelativeScale3D(FVector(5, 5, 5));
	Damage = 30;
	MiniRock = ARock2::StaticClass();
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
	auto MiniRock1 = GetWorld()->SpawnActor<ARock2>(MiniRock, GetActorLocation() + FVector(0, 0, 0), GetActorRotation());
	auto MiniRock2 = GetWorld()->SpawnActor<ARock2>(MiniRock, GetActorLocation() + FVector(0, 0, 0), GetActorRotation());

	MiniRock1->Sphere->AddImpulse(FVector(100000, 100000, 2000000));
	MiniRock2->Sphere->AddImpulse(FVector(-100000, -100000, 2000000));
	
	Destroy();
}


