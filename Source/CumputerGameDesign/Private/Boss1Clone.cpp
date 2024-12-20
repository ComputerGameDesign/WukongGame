// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss1Clone.h"

#include "Boss1.h"
#include "MainGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ABoss1Clone::ABoss1Clone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Boss"));
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCapsuleHalfHeight(96.0f);
	CapsuleComponent->SetCapsuleRadius(60.0f);
	CapsuleComponent->SetRelativeScale3D(FVector(1.75f, 1.75f, 1.75f));
	CapsuleComponent->SetCollisionProfileName("Pawn");
	RootComponent = CapsuleComponent;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSunWukong/Characters/Heroes/Wukong/Meshes/Wukong.Wukong'"));
	if (MeshAsset.Succeeded()) SkeletalMesh->SetSkeletalMesh(MeshAsset.Object);
	
	SkeletalMesh->SetRelativeLocation(FVector(8, 0, -96));
	SkeletalMesh->SetRelativeRotation(FRotator(0, -90, 0));
	SkeletalMesh->SetupAttachment(CapsuleComponent);
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("/Script/Engine.AnimBlueprint'/Game/ABP/ABP_Boss1Clone.ABP_Boss1Clone_C'"));
	if (AnimBlueprint.Succeeded()) SkeletalMesh->SetAnimInstanceClass(AnimBlueprint.Class);
	
	SpawnEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/ParagonSunWukong/FX/Particles/Wukong/Abilities/Ultimate/FX/p_CloneSpawn.p_CloneSpawn'"));
	DespawnEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/ParagonSunWukong/FX/Particles/Wukong/Abilities/Ultimate/FX/p_CloneDespawn.p_CloneDespawn'"));
}

// Called when the game starts or when spawned
void ABoss1Clone::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABoss1Clone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DespawnEffect, GetActorLocation(), GetActorRotation());
	GetWorldTimerManager().ClearTimer(Timer);
}


// Called every frame
void ABoss1Clone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanMove)
	{
		Moving();	
	}
}

void ABoss1Clone::Moving()
{
	constexpr float Delta = 0.1f;
	if (FMath::IsNearlyEqual(GetActorLocation().X, EndPosition.X, Delta) &&
		FMath::IsNearlyEqual(GetActorLocation().Y, EndPosition.Y, Delta))
	{
		CanMove = false;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, GetActorLocation(), GetActorRotation());
		StartTimer();
	}
	else
	{
		MovingDeltaSeconds += GetWorld()->GetDeltaSeconds();
		FVector NewLocation = FMath::VInterpTo(StartPosition, EndPosition, MovingDeltaSeconds, 5.0f);
		NewLocation.Z = GetActorLocation().Z;
		SetActorLocation(NewLocation);
	}
}

void ABoss1Clone::SetMove(const FVector& Location)
{
	MovingDeltaSeconds = 0;
	StartPosition = GetActorLocation();
	StartPosition.Z = 0;
	EndPosition = FVector(Location);
	CanMove = true;
}

void ABoss1Clone::StartTimer()
{
	GetWorldTimerManager().SetTimer(
		Timer,
		[&]() -> void
		{
			auto Boss = GetWorld()->SpawnActor<ABoss1>(ABoss1::StaticClass(), GetActorLocation(), GetActorRotation());
			Boss->Hp = Boss->MaxHp * Boss->PatternCloneHpPercent;
			Boss->CanPatternClone = false;
			Boss->CanPatternRockThrow = false;
			Boss->NowPatternNeutralizeCount = Boss->TotalPatternNeutralizeCount;
			Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode())->BossClones.Add(Boss);
			Destroy();
		},
		PersistentTime,
		false);
}

