// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "Boss1.h"
#include "Boss1Clone.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "MainGameModeBase.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundCue.h"

class USplineComponent;
class ABoss1;
class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Tags.Add(FName("Player"));
	
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(108.0f);
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0, 70, 90));
	SpringArmComp->TargetArmLength = 400;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonRevenant/Characters/Heroes/Revenant/Meshes/Revenant.Revenant'"));
	if (MeshAsset.Succeeded()) GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	GetMesh()->SetRelativeLocation(FVector(10, 0, -108));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/ABP/ABP_MainCharacter.ABP_MainCharacter_C'"));
	if (AnimAsset.Succeeded()) GetMesh()->SetAnimInstanceClass(AnimAsset.Class);

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> ImcPlayerAsset(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_Player.IMC_Player'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IaMoveAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Move.IA_Move'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IaJumpAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Jump.IA_Jump'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IaDashAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Dash.IA_Dash'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IaViewAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_View.IA_View'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IaShootAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Shoot.IA_Shoot'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IaShootAllAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_ShootAll.IA_ShootAll'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> IaReloadAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Reload.IA_Reload'"));
	
	if (ImcPlayerAsset.Succeeded()) Imc_Player = ImcPlayerAsset.Object;
	if (IaMoveAsset.Succeeded()) Ia_Move = IaMoveAsset.Object;
	if (IaJumpAction.Succeeded()) Ia_Jump = IaJumpAction.Object;
	if (IaDashAsset.Succeeded()) Ia_Dash = IaDashAsset.Object;
	if (IaViewAsset.Succeeded()) Ia_View = IaViewAsset.Object;
	if (IaShootAsset.Succeeded()) Ia_Shoot = IaShootAsset.Object;
	if (IaShootAllAsset.Succeeded()) Ia_ShootAll = IaShootAllAsset.Object;
	if (IaReloadAsset.Succeeded()) Ia_Reload = IaReloadAsset.Object;

	ShootEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRevenant/FX/Particles/Revenant/Abilities/Primary/FX/P_Revenant_Primary_MuzzleFlash.P_Revenant_Primary_MuzzleFlash'"));
	ShootTrailEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/ParagonProps/FX/Particles/Core/P_SingleTargetCore_TargetBeam.P_SingleTargetCore_TargetBeam'"));
	ShootHitEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRevenant/FX/Particles/Revenant/Abilities/Obliterate/FX/P_Revenant_Obliterate_Hit.P_Revenant_Obliterate_Hit'"));
	DamagedEffect = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/ParagonRevenant/FX/Particles/Revenant/Abilities/Obliterate/FX/P_Revenant_Obliterate_CamFX.P_Revenant_Obliterate_CamFX'"));
	
	ShootSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sounds/Revenant_Gun_Single_Fire.Revenant_Gun_Single_Fire'"));
	ShootHitSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/SmallSoundKit/SSKCue/DarkCue/Impact_Guts_Gore_01_Cue.Impact_Guts_Gore_01_Cue'"));
	ShootSoundNoBullet = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sounds/rifle-clip-empty-98832.rifle-clip-empty-98832'"));
	ReloadSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sounds/Revenant_Gun_Reload.Revenant_Gun_Reload'"));
	PainSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/ParagonRevenant/Audio/Cues/Revenant_Effort_PainHeavy.Revenant_Effort_PainHeavy'"));
	JumpSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/ParagonRevenant/Audio/Cues/Revenant_Effort_Jump.Revenant_Effort_Jump'"));
	MoveSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/SmallSoundKit/SSKCue/FootstepsCue/S_Concrete_Mono_Cue.S_Concrete_Mono_Cue'"));
	DashSound = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sounds/evade_dash.evade_dash'"));
	DieSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/ParagonRevenant/Audio/Cues/Revenant_Effort_Death.Revenant_Effort_Death'"));
	
	MoveAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	MoveAudioComp->SetSound(MoveSound);
	MoveAudioComp->SetAutoActivate(false);
	MoveAudioComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (const auto Pc = Cast<APlayerController>(Controller))
	{
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Pc->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(Imc_Player, 0);
		}
	}
	
	Hp = MaxHp;
}

void AMainCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MoveDirection != FVector::ZeroVector)
	{
		AddMovementInput(MoveDirection * DeltaTime * Speed);
		MoveDirection = FVector::ZeroVector;

		if (!MoveAudioComp->IsPlaying())
		{
			MoveAudioComp->Play();
		}
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (const auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		PlayerInput->BindAction(Ia_Move, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		PlayerInput->BindAction(Ia_Jump, ETriggerEvent::Started, this, &AMainCharacter::JumpThis);
		PlayerInput->BindAction(Ia_Dash, ETriggerEvent::Started, this, &AMainCharacter::Dash);
		PlayerInput->BindAction(Ia_View, ETriggerEvent::Triggered, this, &AMainCharacter::View);
		PlayerInput->BindAction(Ia_Shoot, ETriggerEvent::Started, this, &AMainCharacter::Shoot);
		PlayerInput->BindAction(Ia_ShootAll, ETriggerEvent::Started, this, &AMainCharacter::ShootAll);
		PlayerInput->BindAction(Ia_Reload, ETriggerEvent::Started, this, &AMainCharacter::Reload);
	}
}

void AMainCharacter::Move(const struct FInputActionValue &inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	MoveDirection = FVector(value.X, value.Y, 0);
	MoveDirection = FTransform(GetControlRotation()).TransformVector(MoveDirection);
	MoveDirection.Z = 0;
	MoveDirection.Normalize();
}

void AMainCharacter::JumpThis(const struct FInputActionValue &inputValue)
{
	if (!IsDashing && !GetCharacterMovement()->IsFalling())
	{
		Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode())->JumpCount++;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), JumpSound, GetActorLocation());
		Super::Jump();
	}
}

void AMainCharacter::Dash(const struct FInputActionValue &inputValue)
{
	if (!IsDashing && !GetCharacterMovement()->IsFalling())
	{
		Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode())->RushCount++;
		const FVector CharacterVelocity = GetVelocity();
		const FVector DashDirection = FVector(CharacterVelocity.X, CharacterVelocity.Y, 0).GetSafeNormal();

		IsDashing = true;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DashSound, GetActorLocation());
		GetWorldTimerManager().SetTimer(DashTimer, [&]() -> void { IsDashing = false; }, DashCoolTime, false);
		LaunchCharacter(DashDirection * DashPower, true, false);
	}
}

void AMainCharacter::View(const struct FInputActionValue &inputValue)
{
	const FVector2D Value = inputValue.Get<FVector2D>();
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void AMainCharacter::Shoot(const struct FInputActionValue &inputValue)
{
	if (!IsShoot && !IsShootAll && !IsReloading)
	{
		if (NowBulletCount > 0)
		{
			IsShoot = true;
			ShootOnce();
			GetWorldTimerManager().SetTimer(
				ShootTimer,
				[&]() -> void
				{
					IsShoot = false;
				},
				ShootDelay,
				false);
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSoundNoBullet, GetActorLocation());
		}
	}
}

void AMainCharacter::ShootOnce()
{
	if (NowBulletCount > 0)
	{
		NowBulletCount--;
		Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode())->ShootCount++;
		
		const FVector FirePos = GetMesh()->GetBoneLocation(FName("gun_pin"));

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),  // 현재 월드
			ShootEffect,  // 사용할 파티클 시스템
			FirePos,
			FRotationMatrix::MakeFromX(GetActorForwardVector()).Rotator()// 생성할 위치
		);

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, FirePos);
		auto Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootTrailEffect, FirePos);

		FVector Start = CameraComp->GetComponentLocation();
		FVector CameraDir = CameraComp->GetForwardVector();
		FVector End = Start + CameraDir * 10000;

		FHitResult HitResult;
		
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this); // 자기 자신 무시

		// 레이캐스트 수행
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, CollisionParams);
		
		if (bHit && HitResult.GetActor()->ActorHasTag(TEXT("Boss")))
		{
			Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode())->HitCount++;
			const FVector HitPos = HitResult.ImpactPoint;

			Beam->SetBeamSourcePoint(0, FirePos, 0);
			Beam->SetBeamTargetPoint(0, HitPos, 0);

			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootHitSound, HitPos);
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),  // 현재 월드
				ShootHitEffect,  // 사용할 파티클 시스템
				HitPos,
				FRotationMatrix::MakeFromX(HitPos - FirePos).Rotator()// 생성할 위치
			);
			
			if (ABoss1* Boss = Cast<ABoss1>(HitResult.GetActor()))
			{
				Boss->TakeDamageToThis(ShootDamage);
			}

			if (ABoss1Clone* Clone = Cast<ABoss1Clone>(HitResult.GetActor()))
			{
				Clone->Destroy();
			}
		}
		else
		{
			if (bHit)
            {
            	const FVector HitPos = HitResult.ImpactPoint;
            	UGameplayStatics::SpawnEmitterAtLocation(
            		GetWorld(),  // 현재 월드
            		ShootHitEffect,  // 사용할 파티클 시스템
            		HitPos,
            		FRotationMatrix::MakeFromX(HitPos - FirePos).Rotator()// 생성할 위치
            	);
            }
			const FVector EndPos = FirePos + CameraDir * 10000;

			Beam->SetBeamSourcePoint(0, FirePos, 0);
			Beam->SetBeamTargetPoint(0, EndPos, 0);
		}
	}
}

void AMainCharacter::ShootAll(const struct FInputActionValue& inputValue)
{
	if (!IsShootAll && !IsReloading)
	{
		if (NowBulletCount > 0)
		{
			IsShootAll = true;
			GetWorldTimerManager().SetTimer(
				ShootAllTimer,
				[&]() -> void
				{
					if (NowBulletCount <= 0)
					{
						IsShootAll = false;
						GetWorldTimerManager().ClearTimer(ShootAllTimer);
					}
					ShootOnce();
				},
				ShootAllDelay,
				true);
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSoundNoBullet, GetActorLocation());
		}
	}
}

void AMainCharacter::Reload(const struct FInputActionValue& inputValue)
{
	if (!IsReloading && NowBulletCount < MaxBulletCount)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSound, GetActorLocation());
		
		IsReloading = true;
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			[&]() -> void
			{
				NowBulletCount = MaxBulletCount;
				IsReloading = false;
			},
			ReloadingDelay,
			false);
	}
}

void AMainCharacter::Die()
{
	IsDie = true;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DieSound, GetActorLocation());
	
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->SetIgnoreLookInput(true);
	}
	
	if (const auto Pc = Cast<APlayerController>(Controller))
	{
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Pc->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
		}
	}
	
	Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode())->PlayerDie();
}

void AMainCharacter::TakeDamageToThis(const float Damage)
{
	if (!IsImmune && Hp > 0)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PainSound, GetActorLocation());
		UGameplayStatics::SpawnEmitterAttached(
			DamagedEffect,
			CameraComp,       // 캐릭터의 스켈레탈 메시
			FName(""), // 소켓 이름
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
		
		Hp = FMath::Max(Hp - Damage, 0.0f);

		if (Hp <= 0)
		{
			Die();
		}
		else
		{
			FVector KnockbackDirection = -GetActorForwardVector() * 500;
			KnockbackDirection.Z = 500.0f;
			LaunchCharacter(KnockbackDirection, true, true);
		
			IsImmune = true;
			GetWorldTimerManager().SetTimer(
				ImmuneTimer,
				[&]() -> void
				{
					IsImmune = false;
				},
				ImmunePersistantTime,
				false);
		}
	}
}
