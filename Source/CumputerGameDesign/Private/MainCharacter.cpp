// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "Boss1.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

class ABoss1;
class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Tags.Add(TEXT("Player"));
	
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

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/ABP_MainCharacter.ABP_MainCharacter'"));
	if (AnimAsset.Succeeded()) GetMesh()->SetAnimInstanceClass(AnimAsset.Object->GeneratedClass);
	
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
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddMovementInput(MoveDirection * DeltaTime * Speed);
	MoveDirection = FVector::ZeroVector;

	DrawDebug();
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (const auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		PlayerInput->BindAction(Ia_Move, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		PlayerInput->BindAction(Ia_Jump, ETriggerEvent::Started, this, &AMainCharacter::Jump);
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

void AMainCharacter::Jump(const struct FInputActionValue &inputValue)
{
	if (!IsDashing)
	{
		Super::Jump();
	}
}

void AMainCharacter::Dash(const struct FInputActionValue &inputValue)
{
	if (!IsDashing && !GetCharacterMovement()->IsFalling())
	{
		const FVector CharacterVelocity = GetVelocity();
		const FVector DashDirection = FVector(CharacterVelocity.X, CharacterVelocity.Y, 0).GetSafeNormal();

		IsDashing = true;
		GetWorldTimerManager().SetTimer(Timer, [&]() -> void { IsDashing = false; }, DashCoolTime, false);
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
}

void AMainCharacter::ShootOnce()
{
	if (NowBulletCount > 0)
	{
		NowBulletCount--;
		
		const FVector Start = GetMesh()->GetBoneLocation(FName("gun_pin"));
		const FVector ForwardVector = GetControlRotation().Vector();
		const FVector End = Start + (ForwardVector * 10000.0f);

		// 충돌 정보를 저장할 FHitResult 구조체
		FHitResult HitResult;

		// 충돌 채널 설정 (기본적으로는 ECC_Visibility 사용)
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this); // 자기 자신 무시

		// 레이캐스트 수행
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, CollisionParams);

		// 디버그용 레이 표시
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.0f, 0, 1.0f);

		if (bHit && HitResult.GetActor()->ActorHasTag(TEXT("Boss")))
		{
			if (ABoss1* Boss = Cast<ABoss1>(HitResult.GetActor()))
			{
				Boss->TakeDamage(ShootDamage);
			}
			
			// 충돌한 액터 정보 출력
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());

			// 충돌 위치에 디버그 스피어 그리기
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 12, FColor::Blue, false, 1.0f);
		}
	}
}

void AMainCharacter::ShootAll(const struct FInputActionValue& inputValue)
{
	if (!IsShootAll && !IsReloading)
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
}

void AMainCharacter::Reload(const struct FInputActionValue& inputValue)
{
	if (!IsReloading)
	{
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

void AMainCharacter::TakeDamage(const float Damage)
{
	Hp = FMath::Max(Hp - Damage, 0.0f);
	// immune
}

void AMainCharacter::DrawDebug() const
{
	FVector Start = GetMesh()->GetBoneLocation(FName("gun_pin"));
	FVector ForwardVector = GetControlRotation().Vector();
	FVector End = Start + (ForwardVector * 50.0f); // 1000 단위 거리까지 레이캐스트
	DrawDebugDirectionalArrow(GetWorld(), Start, End, 50.0f, FColor::Red, false, -1, 0, 1.0f);
	
}

