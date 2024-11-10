// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss1Clone.generated.h"

UCLASS()
class CUMPUTERGAMEDESIGN_API ABoss1Clone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoss1Clone();

	UPROPERTY(EditAnywhere, Category=CapsuleComponent)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, Category=SkeletalMesh)
	class USkeletalMeshComponent* SkeletalMesh;
	
	UPROPERTY(EditAnywhere, Category=GamePlay)
	float PersistentTime;

	UPROPERTY(VisibleInstanceOnly, Category=Gameplay)
	bool CanMove = false;

	UPROPERTY(VisibleInstanceOnly, Category=Gameplay)
	FVector StartPosition;

	UPROPERTY(VisibleInstanceOnly, Category=Gameplay)
	FVector EndPosition;

	UPROPERTY(VisibleInstanceOnly, Category=Gameplay)
	float MovingDeltaSeconds;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetMove(const FVector& Location);
	
	UFUNCTION()
	void StartTimer();

private:
	void Moving();

	FTimerHandle Timer;
};
