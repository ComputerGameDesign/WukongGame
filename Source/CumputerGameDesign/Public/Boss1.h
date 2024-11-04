// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Boss1.generated.h"

UENUM()
enum class EBossState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Casting UMETA(DisplayName = "Casting"),
	Jumping UMETA(DisplayName = "Jumping"),
	RushTracing UMETA(DisplayName = "RushTracing"),
	Rushing UMETA(DisplayName = "Rushing"),
	AttackMoving UMETA(DisplayName = "AttackMoving"),
	Attack UMETA(DisplayName = "Attack"),
	Groggy UMETA(DisplayName = "Groggy"),
	Die UMETA(DisplayName = "Die")
};

UCLASS()
class CUMPUTERGAMEDESIGN_API ABoss1 : public ACharacter
{
	GENERATED_BODY()

public:
	ABoss1();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere)
	class AMainCharacter* TargetPlayer;

	UPROPERTY(EditAnyWhere)
	class UCapsuleComponent* WeaponCollider;

	UPROPERTY(VisibleAnywhere, Category=State)
	EBossState State = EBossState::Idle;

	UPROPERTY(VisibleAnywhere, Category=State)
	bool CanJumpToPlayer = true;
	
	UPROPERTY(VisibleAnywhere, Category=State)
	bool CanRush = true;

	UPROPERTY(VisibleAnywhere, Category=State)
	bool CanAttack = true;
	
	UPROPERTY(EditAnywhere, Category=Values)
	float Speed = 1000.0f;

	UPROPERTY(EditAnywhere, Category=Values)
	float Hp;
	
	UPROPERTY(EditAnywhere, Category=Values)
	float JumpZLocation = 900.0f;
	
	UPROPERTY(EditAnywhere, Category=Values)
	float JumpLandingVelocity = 7500.0f;

	UPROPERTY(EditAnywhere, Category=Values)
	float JumpCastingDelay = 0.5f;

	UPROPERTY(EditAnywhere, Category=Values)
	float JumpLandingDelay = 0.5f;
	
	UPROPERTY(EditAnywhere, Category=Values)
	float JumpDistanceThreshold = 1000.0f;

	UPROPERTY(EditAnywhere, Category=Values)
	float JumpCoolTime = 5.0f;

	UPROPERTY(EditAnywhere, Category=Values)
	float JumpDamage;
	
	UPROPERTY(EditAnywhere, Category=Values)
	FVector JumpStartPosition;
	
	UPROPERTY(EditAnywhere, Category=Values)
	FVector JumpTargetPosition;

	UPROPERTY(EditAnywhere, Category=Values)
	float JumpDeltaSeconds = 0;

	UPROPERTY(EditAnywhere, Category=Values)
	float RushTracingTime = 3.0f;
	
	UPROPERTY(EditAnywhere, Category=Values)
	float RushSpeed = 5000.0f;

	UPROPERTY(EditAnywhere, Category=Values)
	float RushDistanceThreshold = 1000.0f;

	UPROPERTY(EditAnywhere, Category=Values)
	float RushCoolTime = 15.0f;

	UPROPERTY(EditAnywhere, Category=Values)
	float RushDamage;

	UPROPERTY(EditAnywhere, Category=Values)
	float AttackDistanceThreshold = 750.0f;

	UPROPERTY(EditAnywhere, Category=Values)
	float AttackCoolTime = 1.0f;

	UPROPERTY(EditAnywhere, Category=Values)
	float AttackMovingCastingDelay = 0.5f;

	UPROPERTY(EditAnywhere, Category=Values)
	float AttackCastingDelay = 0.5f;
	
	UPROPERTY(EditAnywhere, Category=Values)
	float AttackDelay = 1.5f;
	
	UPROPERTY(EditAnywhere, Category=Values)
	float AttackFinishDelay = 0.5f;

	UPROPERTY(EditAnywhere, Category=Values)
	float AttackDamage;

	UPROPERTY(EditAnywhere, Category=Values)
	FVector AttackStartPosition;

	UPROPERTY(EditAnywhere, Category=Values)
	FVector AttackTargetPosition;

	UPROPERTY(EditAnywhere, Category=Values)
	float AttackDeltaSeconds;
	
	UPROPERTY(EditAnywhere, Category=Values)
	float GroggyPersistenceTime = 3.0f;
	
	UFUNCTION()
	FVector GetTargetDirection() const;

	UFUNCTION()
	FVector GetTargetDirectionWithoutZ() const;

	UFUNCTION()
	void SetActorRotationSmooth(FRotator EndRotation, float InterpSpeed);

	UFUNCTION()
	void SetActorRotationSmoothOnce(const FRotator& EndRotation, float InterpSpeed, float DeltaTime);

	UFUNCTION()
	void SetCoolTime(bool &CanValue, float CoolTime) const;

	UFUNCTION()
	void TakeDamage(const float Damage);
	

private:
	void CheckState();
	void IdleTransition();
	void Chase();
	void StartJumping();
	void Jumping();
	void Landing();
	void StartRushing();
	void RushTracing();
	void Rushing();
	void StartGroggy();
	void StartAttack();
	void AttackMoving();
	void AttackMeleeOnce();
	void AttackHorizontal();
	void Attacking();
	void AttackVertical();
	void AttackPush();

	UFUNCTION()
	void OnBodyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void DrawDebug() const;

	FTimerHandle SmoothTimer;
};
