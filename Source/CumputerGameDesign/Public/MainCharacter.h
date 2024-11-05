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
	float ShootDamage = 1.0f;

	UPROPERTY(EditAnywhere, Category = GamePlay)
	float ShootDelay = 0.5f;
	
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
	float ReloadingDelay = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = GamePlay)
	bool IsReloading = false;
	
	UPROPERTY(EditAnywhere, Category = GamePlay)
	float Speed = 500.0f;

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

	FTimerHandle Timer;
	FTimerHandle ShootTimer;
	FTimerHandle ShootAllTimer;
	FVector MoveDirection;

	void DrawDebug() const;
};
