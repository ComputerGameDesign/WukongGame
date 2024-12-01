// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss1.h"

#include "EngineUtils.h"
#include "MainCharacter.h"
#include "MainGameModeBase.h"
#include "ShaderPrintParameters.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
	
// Sets default values
ABoss1::ABoss1()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Boss"));

	Clone = ABoss1Clone::StaticClass();
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(60.0f);
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.75f, 1.75f, 1.75f));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSunWukong/Characters/Heroes/Wukong/Meshes/Wukong.Wukong'"));
	if (MeshAsset.Succeeded()) GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	GetMesh()->SetRelativeLocation(FVector(8, 0, -96));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("/Script/Engine.AnimBlueprint'/Game/ABP/ABP_Boss1.ABP_Boss1_C'"));
	if (AnimBlueprint.Succeeded()) GetMesh()->SetAnimInstanceClass(AnimBlueprint.Class);
	
	WeaponCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollider"));
	WeaponCollider->SetupAttachment(RootComponent);
	WeaponCollider->SetCapsuleSize(5.0f, 120.0f);

	SpawnSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/monster-roar-104049_Cue.monster-roar-104049_Cue'"));
	RushTracingStartSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/monster-growl-94644_Cue.monster-growl-94644_Cue'"));
	RushStartSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/swoosh-6-235279_Cue.swoosh-6-235279_Cue'"));
	RushingHitSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/debris-break-253779_Cue.debris-break-253779_Cue'"));
	JumpingSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/jumpland_Cue.jumpland_Cue'"));
	LandingSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/shake_Cue.shake_Cue'"));
	MeleeAttackSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/swing_whoosh-weapon.swing_whoosh-weapon'"));
	NeutralizeSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/magic-chargeup-102051_Cue.magic-chargeup-102051_Cue'"));
	NeutralizeSuccessSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/glass-shatter-3-100155_Cue.glass-shatter-3-100155_Cue'"));
	RockThrowStartSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/MonsterRoarsAndGrowls/cues/05_Chilling_Roar_Cue.05_Chilling_Roar_Cue'"));
	RockThrowSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/near-miss-swing-whoosh-3-233426_Cue.near-miss-swing-whoosh-3-233426_Cue'"));
	CloningSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sounds/woosh-104586_Cue.woosh-104586_Cue'"));
	DieSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/ParagonSunWukong/Audio/Cues/Wukong_Effort_Death.Wukong_Effort_Death'"));

	NeutralizeAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	NeutralizeAudioComp->SetSound(NeutralizeSound);
	NeutralizeAudioComp->SetAutoActivate(false);
	NeutralizeAudioComp->SetupAttachment(RootComponent);
	
	LandingEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/ParagonSunWukong/FX/Particles/Wukong/Abilities/DoubleJump/FX/p_GroundSlam_Radius.p_GroundSlam_Radius'"));
	NeutralizeFailEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/ParagonProps/FX/Particles/Core/P_Core_Destroyed_MainBlast.P_Core_Destroyed_MainBlast'"));
	RushHitEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/ParagonSunWukong/FX/Particles/Wukong/Abilities/DoubleJump/FX/p_GroundSlam_StaffImpact.p_GroundSlam_StaffImpact'"));
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

	State = EBossState::Spawn;

	SetActorRotationSmooth(GetTargetDirectionWithoutZ().Rotation(), 20.0f);
}

void ABoss1::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(SmoothTimer);
	GetWorld()->GetTimerManager().ClearTimer(PatternTimer);
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

void ABoss1::TakeDamageToThis(float Damage)
{
	if (State != EBossState::PatternCloning && State != EBossState::Spawn)
	{
		if (State == EBossState::Groggy)
			Damage *= GroggyDamageMultiplier;

		Shield = FMath::Max(Shield - Damage, 0.0f);
		Damage = FMath::Max(Damage - Shield, 0.0f);
		Hp = FMath::Max(Hp - Damage, 0.0f);

		if (Hp <= 0)
		{
			Die();
		}
	}
}

void ABoss1::Spawning()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnSound, GetActorLocation());
		State = EBossState::Casting;
		GetWorldTimerManager().SetTimer(
		PatternTimer,
		[&]() -> void { State = EBossState::Idle; },
		SpawningTime,
		false);
	}
}

void ABoss1::Die()
{
	State = EBossState::Die;
	GetWorldTimerManager().ClearTimer(SmoothTimer);
	GetWorldTimerManager().ClearTimer(PatternTimer);

	if (this == Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode())->Boss)
	{
		Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode())->BossDie();
	}
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DieSound, GetActorLocation());
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		[&]() -> void { Destroy(); },
		3.0f,
		false);
}

void ABoss1::CheckState()
{
	switch (State)
	{
	case EBossState::Spawn:
		Spawning();
		break;
		
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
	GetWorldTimerManager().SetTimer(
		PatternTimer,
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
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RushHitEffect, Hit.Location, FRotationMatrix::MakeFromX(Hit.Normal).Rotator());
			if (OtherActor->ActorHasTag(FName("Wall")))
			{
				UGameplayStatics::PlaySoundAtLocation(this, RushingHitSound, GetActorLocation());
				StartGroggy();
			}
			else if (OtherActor->ActorHasTag(FName("Player")))
			{
				UGameplayStatics::PlaySoundAtLocation(this, RushingHitSound, GetActorLocation());
				TargetPlayer->TakeDamageToThis(RushDamage);
				State = EBossState::Idle;
			}
		}
		else if (State == EBossState::Landing && OtherActor->ActorHasTag(FName("Player")))
		{
			TargetPlayer->TakeDamageToThis(JumpDamage);
		}
	}
}

void ABoss1::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (State == EBossState::Attack && OtherActor->ActorHasTag(FName("Player")))
		{
			TargetPlayer->TakeDamageToThis(AttackDamage);
			UE_LOG(LogTemp, Warning, TEXT("Damage"))
		}
	}
}
