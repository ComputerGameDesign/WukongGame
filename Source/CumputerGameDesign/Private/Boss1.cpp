// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss1.h"

#include "EngineUtils.h"
#include "MainCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABoss1::ABoss1()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Boss"));
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.75f, 1.75f, 1.75f));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSunWukong/Characters/Heroes/Wukong/Meshes/Wukong.Wukong'"));
	if (MeshAsset.Succeeded()) GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	GetMesh()->SetRelativeLocation(FVector(8, 0, -96));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprint(TEXT("/Script/Engine.AnimBlueprint'/Game/ABP_Boss1.ABP_Boss1'"));
	if (AnimBlueprint.Succeeded()) GetMesh()->SetAnimInstanceClass(AnimBlueprint.Object->GeneratedClass);
	
	WeaponCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollider"));
	WeaponCollider->SetupAttachment(RootComponent);
	WeaponCollider->SetCapsuleSize(5.0f, 120.0f);
}	

// Called when the game starts or when spawned
void ABoss1::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();

	for (TActorIterator<AMainCharacter> It(GetWorld()); It; ++It)
	{
		TargetPlayer = *It;
	}
	
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ABoss1::OnBodyHit);
	WeaponCollider->OnComponentBeginOverlap.AddDynamic(this, &ABoss1::OnWeaponBeginOverlap);

	SetActorEnableCollision(true);
	Hp = MaxHp;
}

// Called every frame
void ABoss1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckState();
	
	//DrawDebug();
}

// Called to bind functionality to input
void ABoss1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FVector ABoss1::GetTargetDirection() const
{
	const FVector Direction = TargetPlayer->GetActorLocation() - GetActorLocation();
	return Direction;
}

FVector ABoss1::GetTargetDirectionWithoutZ() const
{
	FVector Direction = TargetPlayer->GetActorLocation() - GetActorLocation();
	Direction.Z = 0;
	return Direction;
}

void ABoss1::SetActorRotationSmooth(FRotator EndRotation, float InterpSpeed)
{
	constexpr float Rate = 0.001f;
	constexpr float DeltaAngle = 0.1f;
	GetWorldTimerManager().SetTimer(
		SmoothTimer,
		[&, EndRotation, InterpSpeed, Rate, DeltaAngle]() -> void
		{
			SetActorRotationSmoothOnce(EndRotation, InterpSpeed, Rate);
			
			if (FMath::IsNearlyEqual(GetActorRotation().Yaw, EndRotation.Yaw, DeltaAngle) &&
				FMath::IsNearlyEqual(GetActorRotation().Pitch, EndRotation.Pitch, DeltaAngle) &&
				FMath::IsNearlyEqual(GetActorRotation().Roll, EndRotation.Roll, DeltaAngle))
			{
				GetWorldTimerManager().ClearTimer(SmoothTimer);
			}
		},
		Rate,
		true);
}

void ABoss1::SetActorRotationSmoothOnce(const FRotator& EndRotation, const float InterpSpeed, const float DeltaTime)
{
	const FRotator CurrentRotation = GetActorRotation();
	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, EndRotation, DeltaTime, InterpSpeed);
	        
	SetActorRotation(NewRotation);
}

void ABoss1::SetCoolTime(bool &CanValue, const float CoolTime) const
{
	CanValue = false;
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		[&]() -> void { CanValue = true; },
		CoolTime,
		false);
}

void ABoss1::TakeDamage(float Damage)
{
	if (State == EBossState::Groggy)
		Damage *= GroggyDamageMultiplier;

	Shield = FMath::Max(Shield - Damage, 0.0f);
	Damage = FMath::Max(Damage - Shield, 0.0f);
	Hp = FMath::Max(Hp - Damage, 0.0f);
}

void ABoss1::CheckState()
{
	switch (State)
	{
	case EBossState::Idle:
		IdleTransition();
		break;
	
	case EBossState::Jumping:
		Jumping();
		break;

	case EBossState::Landing:
		Landing();
		break;
		
	case EBossState::RushTracing:
		RushTracing();
		break;

	case EBossState::Rushing:
		Rushing();
		break;

	case EBossState::AttackMoving:
		AttackMoving();
		break;

	case EBossState::Attack:
		Attacking();
		break;

	case EBossState::PatternNeutralizeJumping:
		PatternNeutralizeJumping();
		break;

	case EBossState::PatternNeutralizeLanding:
		PatternNeutralizeLanding();
		break;

	case EBossState::Neutralized:
		PatternNeutralize();
		break;

	case EBossState::PatternRockThrowJumping:
		PatternRockThrowJumping();
		break;

	case EBossState::PatternRockThrowLanding:
		PatternRockThrowLanding();
		break;

	case EBossState::PatternCloneJumping:
		PatternCloneJumping();
		break;

	case EBossState::PatternCloneLanding:
		PatternCloneLanding();
		break;
		
	default:
		break;
	}
}

void ABoss1::IdleTransition()
{
	if (CanPatternClone && Hp / MaxHp <= PatternCloneHpPercent)
	{
		StartPatternCloneJumping();
	}
	else if (CanPatternRockThrow && Hp / MaxHp <= PatternRockThrowHpPercent)
	{
		StartPatternRockThrowJumping();
	}
	else if (NowPatternNeutralizeCount < TotalPatternNeutralizeCount &&
		Hp / MaxHp <= PatternNeutralizeHpPercents[NowPatternNeutralizeCount])
	{
		StartPatternNeutralizeJumping();
	}
	else if (CanRush)
	{
		StartRushing();
	}
	else if (CanJumpToPlayer)
	{
		StartJumping();
	}
	else if (CanAttack)
	{
		StartAttack();
	}
}

void ABoss1::StartGroggy()
{
	State = EBossState::Groggy;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		[&]() -> void { State = EBossState::Idle; },
		GroggyPersistenceTime,
		false);
}


void ABoss1::OnBodyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		if (State == EBossState::Rushing)
		{
			if (OtherActor->ActorHasTag(FName("Wall")))
			{
				StartGroggy();
			}
			else if (OtherActor->ActorHasTag(FName("Player")))
			{
				TargetPlayer->TakeDamage(RushDamage);
				State = EBossState::Idle;
			}
		}
		else if (State == EBossState::Landing && OtherActor->ActorHasTag(FName("Player")))
		{
			TargetPlayer->TakeDamage(JumpDamage);
		}
	}
}

void ABoss1::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (State == EBossState::Attack && OtherActor->ActorHasTag(FName("Player")))
		{
			TargetPlayer->TakeDamage(AttackDamage);
			UE_LOG(LogTemp, Warning, TEXT("Damage"))
		}
	}
}

void ABoss1::DrawDebug() const
{/*
	FColor color;
	if (GetTargetDirection().Size() > JumpDistanceThreshold)
		color = FColor::Red;
	else
		color = FColor::Green;
	
	DrawDebugLine(GetWorld(), GetActorLocation(), TargetPlayer->GetActorLocation(), color, false, -1, 0, 1.0f);
	
	DrawDebugSphere(GetWorld(), GetActorLocation(), AttackDistanceThreshold, 12, FColor::Red, false, -1, 0, 1.0f);
	DrawDebugSphere(GetWorld(), GetActorLocation(), JumpDistanceThreshold, 12, FColor::Green, false, -1, 0, 1.0f);
	DrawDebugCapsule(
		GetWorld(),
		WeaponCollider->GetComponentLocation(),
		WeaponCollider->GetScaledCapsuleHalfHeight(),
		WeaponCollider->GetScaledCapsuleRadius(),
		WeaponCollider->GetComponentRotation().Quaternion(),
		FColor::Yellow,
		false,
		-1,
		0,
		1.0f);

	
	DrawDebugSphere(GetWorld(), JumpTargetPosition, 500, 12, FColor::Blue, false, -1, 0, 1.0f);*/
}
