// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss1Clone.h"
#include "Rock1.h"
#include "GameFramework/Character.h"
#include "Boss1.generated.h"

UENUM()
enum class EBossState : uint8
{
	Spawn UMETA(DisplayName = "Spawn"),
	Idle UMETA(DisplayName = "Idle"),
	Casting UMETA(DisplayName = "Casting"),
	Jumping UMETA(DisplayName = "Jumping"),
	Landing UMETA(DisplayName = "Landing"),
	RushTracing UMETA(DisplayName = "RushTracing"),
	Rushing UMETA(DisplayName = "Rushing"),
	AttackMoving UMETA(DisplayName = "AttackMoving"),
	Attack UMETA(DisplayName = "Attack"),
	Groggy UMETA(DisplayName = "Groggy"),
	Neutralized UMETA(DisplayName = "Neutralized"),
	PatternNeutralizeJumping UMETA(DisplayName = "PatternNeutralizeJumping"),
	PatternNeutralizeLanding UMETA(DisplayName = "PatternNeutralizeLanding"),
	PatternRockThrowJumping UMETA(DisplayName = "PatternRockThrowJumping"),
	PatternRockThrowLanding UMETA(DisplayName = "PatternRockThrowLanding"),
	PatternCloning UMETA(DisplayName = "PatternCloning"),
	PatternCloneJumping UMETA(DisplayName = "PatternCloneJumping"),
	PatternCloneLanding UMETA(DisplayName = "PatternCloneLanding"),
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

	UPROPERTY(EditAnywhere, Category=State)
	mutable EBossState State = EBossState::Idle;
	
	UPROPERTY(VisibleAnywhere, Category=State)
	bool CanJumpToPlayer = true;
	
	UPROPERTY(VisibleAnywhere, Category=State)
	bool CanRush = true;

	UPROPERTY(VisibleAnywhere, Category=State)
	bool CanAttack = true;
	
	UPROPERTY(EditAnywhere, Category=GamePlay)
	float SpawningTime = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	float MaxHp = 200.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	float Hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	float NowMaxShield = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	float Shield = 0.0f;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|Jump")
	float JumpZLocation = 900.0f;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|Jump")
	float JumpLandingVelocity = 7500.0f;

	UPROPERTY(EditAnywhere, Category="Gameplay|Jump")
	float JumpCastingDelay = 0.2f;

	UPROPERTY(EditAnywhere, Category="Gameplay|Jump")
	float JumpLandingDelay = 0.5f;

	UPROPERTY(EditAnywhere, Category="Gameplay|Jump")
	float LandingDelay = 0.5f;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|Jump")
	float JumpCoolTime = 5.0f;

	UPROPERTY(EditAnywhere, Category="Gameplay|Jump")
	float JumpDamage = 30.0f;
	
	UPROPERTY(VisibleInstanceOnly, Category="Gameplay|Jump")
	FVector JumpStartPosition;
	
	UPROPERTY(VisibleInstanceOnly, Category="Gameplay|Jump")
	FVector JumpTargetPosition;

	UPROPERTY(VisibleInstanceOnly, Category="Gameplay|Jump")
	float JumpDeltaSeconds = 0;

	UPROPERTY(EditAnywhere, Category="Gameplay|Rush")
	float RushTracingTime = 3.0f;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|Rush")
	float RushSpeed = 8000.0f;

	UPROPERTY(EditAnywhere, Category="Gameplay|Rush")
	float RushCoolTime = 15.0f;

	UPROPERTY(EditAnywhere, Category="Gameplay|Rush")
	float RushDamage = 50.0f;

	UPROPERTY(EditAnywhere, Category="Gameplay|Attack")
	float AttackCoolTime = 1.0f;

	UPROPERTY(EditAnywhere, Category="Gameplay|Attack")
	float AttackMovingCastingDelay = 0.1f;

	UPROPERTY(EditAnywhere, Category="Gameplay|Attack")
	float AttackCastingDelay = 0.2f;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|Attack")
	float AttackDelay = 1.3f;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|Attack")
	float AttackFinishDelay = 0.2f;

	UPROPERTY(EditAnywhere, Category="Gameplay|Attack")
	float AttackDamage = 10.0f;

	UPROPERTY(VisibleInstanceOnly, Category="Gameplay|Attack")
	FVector AttackStartPosition;

	UPROPERTY(VisibleInstanceOnly, Category="Gameplay|Attack")
	FVector AttackTargetPosition;

	UPROPERTY(VisibleInstanceOnly, Category="Gameplay|Attack")
	float AttackDeltaSeconds;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|Groggy")
	float GroggyPersistenceTime = 3.0f;

	UPROPERTY(EditAnywhere, Category="Gameplay|Groggy")
	float GroggyDamageMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, Category="Gameplay|Neutralize")
	int32 TotalPatternNeutralizeCount = 3;

	UPROPERTY(VisibleInstanceOnly, Category="Gameplay|Neutralize")
	int32 NowPatternNeutralizeCount = 0;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|Neutralize")
	TArray<float> PatternNeutralizeHpPercents = { 0.8f, 0.6f, 0.4f };

	UPROPERTY(EditAnywhere, Category="Gameplay|Neutralize")
	TArray<float> PatternNeutralizeShields = { 10.0f, 10.0f, 12.0f };

	UPROPERTY(EditAnywhere, Category="Gameplay|Neutralize")
	float PatternNeutralizeDelay = 0.5f;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|Neutralize")
	float PatternNeutralizePersistentTime = 10.0f;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|Neutralize")
	float PatternNeutralizeSuccessGroggyTime = 6.0f;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|Neutralize")
	float PatternNeutralizeFailDamage = 40.0f;

	UPROPERTY(EditAnywhere, Category="Gameplay|RockThrow")
	TSubclassOf<ARock1> Rock = ARock1::StaticClass();

	UPROPERTY(EditAnywhere, Category="Gameplay|RockThrow")
	bool CanPatternRockThrow = true;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|RockThrow")
	float PatternRockThrowHpPercent = 0.2f;

	UPROPERTY(EditAnywhere, Category="Gameplay|RockThrow")
	int32 PatternRockThrowTotalRockCount = 10;
	
	UPROPERTY(VisibleInstanceOnly, Category="Gameplay|RockThrow")
	int32 PatternRockThrowNowRockCount = 0;

	UPROPERTY(EditAnywhere, Category="Gameplay|RockThrow")
	float PatternRockThrowStartDelay = 1.5f;

	UPROPERTY(EditAnywhere, Category="Gameplay|RockThrow")
	float PatternRockThrowEndDelay = 1.5f;

	UPROPERTY(EditAnywhere, Category="Gameplay|RockThrow")
	float PatternRockThrowThrowStartDelay = 0.5f;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|RockThrow")
	float PatternRockThrowThrowEndDelay = 1.0f;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|Clone")
	TSubclassOf<ABoss1Clone> Clone;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|Clone")
	bool CanPatternClone = true;
	
	UPROPERTY(EditAnywhere, Category="Gameplay|Clone")
	float PatternCloneHpPercent = 0.1f;

	UPROPERTY(EditAnywhere, Category="Gameplay|Clone")
	int32 PatternCloneCount = 3;

	UPROPERTY(EditAnywhere, Category="Gameplay|Clone")
	TArray<FVector> PatternClonePositions = {
		FVector(1500, 0, 0),
		FVector(-750, 1300, 0),
		FVector(-750, -1300, 0) };

	UPROPERTY(EditAnywhere, Category="Gameplay|Clone")
	float PatternClonePersistentTime = 5.0f;

	UPROPERTY(EditAnywhere, Category="Sounds")
	class USoundCue* SpawnSound;

	UPROPERTY(EditAnywhere, Category="Sounds")
	class USoundCue* JumpingSound;
	
	UPROPERTY(EditAnywhere, Category="Sounds")
	class USoundCue* LandingSound;

	UPROPERTY(EditAnywhere, Category="Sounds")
	class USoundCue* MeleeAttackSound;
	
	UPROPERTY(EditAnywhere, Category="Sounds")
	class USoundCue* RushStartSound;

	UPROPERTY(EditAnywhere, Category="Sounds")
	class USoundCue* NeutralizeSound;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	class UAudioComponent* NeutralizeAudioComp;
	
	UPROPERTY(EditAnywhere, Category="Sounds")
	class USoundCue* NeutralizeSuccessSound;
	
	UPROPERTY(EditAnywhere, Category="Sounds")
	class USoundCue* RockThrowSound;

	UPROPERTY(EditAnywhere, Category="Sounds")
	class USoundCue* CloningSound;

	UPROPERTY(EditAnywhere, Category="Effects")
	class UParticleSystem* LandingEffect;

	UPROPERTY(EditAnywhere, Category="Effects")
	class UParticleSystem* NeutralizeFailEffect;

	UPROPERTY(EditAnywhere, Category="Effects")
	class UParticleSystem* RushHitEffect;
	
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
	void TakeDamage(float Damage);
	

private:
	void Spawning();
	void Die();
	void CheckState();
	void IdleTransition();
	void StartJumping();
	void Jumping();
	void StartLanding();
	void Landing();
	void StartRushing();
	void RushTracing();
	void Rushing();
	void StartGroggy();
	void StartAttack();
	void AttackMoving();
	void AttackMeleeOnce();
	void Attacking();
	void StartPatternNeutralizeJumping();
	void PatternNeutralizeJumping();
	void StartPatternNeutralizeLanding();
	void PatternNeutralizeLanding();
	void StartPatternNeutralize();
	void PatternNeutralize();
	void StartPatternRockThrowJumping();
	void PatternRockThrowJumping();
	void StartPatternRockThrow();
	void StartPatternRockThrowLanding();
	void PatternRockThrowLanding();
	void StartPatternCloneJumping();
	void PatternCloneJumping();
	void StartPatternCloneLanding();
	void PatternCloneLanding();
	void StartPatternClone();

	UFUNCTION()
	void OnBodyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void DrawDebug() const;
	
	FTimerHandle SmoothTimer;
	FTimerHandle PatternTimer;
};
