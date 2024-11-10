// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock2.h"
#include "Components/SphereComponent.h"

// Sets default values
ARock2::ARock2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Sphere->SetRelativeScale3D(FVector(2.5, 2.5, 2.5));
	Damage = 20;
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
	Destroy();
}


