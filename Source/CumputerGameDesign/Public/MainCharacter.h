// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM()
enum class EPlayerState : uint8
{
	Idle UMETA(DisplayName = "Idle")
};

UCLASS()
class CUMPUTERGAMEDESIGN_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* CameraComp;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* Imc_Player;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* Ia_Move;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* Ia_Jump;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* Ia_Dash;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* Ia_View;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* Ia_Shoot;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* Ia_ShootAll;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* Ia_Reload;

	UPROPERTY(EditAnywhere, Category = GamePlay)
	float DashPower = 2500.0f;

	UPROPERTY(EditAnywhere, Category = GamePlay)
	float DashCoolTime = 0.3f;
	
	UPROPERTY(EditAnywhere, Category = GamePlay)
	bool IsDashing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= GamePlay)
	float MaxHp = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
	float Hp;
	
	UPROPERTY(EditAnywhere, Category = GamePlay)
	bool IsImmune = false;
	
	UPROPERTY(EditAnywhere, Category = GamePlay)
	float ImmunePersistantTime = 3.0f;

	UPROPERTY(EditAnywhere, Category = GamePlay)
	float ShootDamage = 1.0f;

	UPROPERTY(EditAnywhere, Category = GamePlay)
	float ShootDelay = 0.4f;
	
	UPROPERTY(EditAnywhere, Category = GamePlay)
	bool IsShoot = false;
	
	UPROPERTY(EditAnywhere, Category = GamePlay)
	float ShootAllDelay = 0.1f;

	UPROPERTY(EditAnywhere, Category = GamePlay)
	bool IsShootAll = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
	int32 MaxBulletCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
	int32 NowBulletCount = 4;

	UPROPERTY(EditAnywhere, Category = GamePlay)
	float ReloadingDelay = 2.8f;
	
	UPROPERTY(EditAnywhere, Category = GamePlay)
	bool IsReloading = false;
	
	UPROPERTY(EditAnywhere, Category = GamePlay)
	float Speed = 500.0f;

	UPROPERTY(EditAnywhere, Category = GamePlay)
	bool IsDie = false;
	
	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* ShootEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* ShootTrailEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* DamagedEffect;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundWave* ShootSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundWave* ShootSoundNoBullet;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundWave* ReloadSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* PainSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* JumpSound;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundWave* DashSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	UAudioComponent* MoveAudioComp;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* MoveSound;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* DieSound;

	UPROPERTY(VisibleInstanceOnly, Category = "Sound")
	bool IsMoveSoundPlaying = false;
	
	UFUNCTION()
	void TakeDamage(const float Damage);

private:
	void Move(const struct FInputActionValue &inputValue);
	void Jump(const struct FInputActionValue &inputValue);
	void Dash(const struct FInputActionValue &inputValue);
	void View(const struct FInputActionValue &inputValue);
	void Shoot(const struct FInputActionValue &inputValue);
	void ShootOnce();
	void ShootAll(const struct FInputActionValue &inputValue);
	void Reload(const struct FInputActionValue &inputValue);
	void Die();

	FTimerHandle DashTimer;
	FTimerHandle ImmuneTimer;
	FTimerHandle ShootTimer;
	FTimerHandle ShootAllTimer;
	FVector MoveDirection;

	void DrawDebug() const;
};
