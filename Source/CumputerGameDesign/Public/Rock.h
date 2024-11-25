// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rock.generated.h"

UCLASS(Abstract)
class CUMPUTERGAMEDESIGN_API ARock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category=SphereComponent)
	class USphereComponent* Sphere;
	
	UPROPERTY(EditAnywhere, Category=Mesh)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category=Damage)
	float Damage;

	UPROPERTY(EditAnywhere, Category=Damage)
	bool IsBreakable = true;

	UPROPERTY(EditAnywhere, Category=Damage)
	TSubclassOf<ARock> MiniRock;

	UPROPERTY(EditAnywhere, Category=Sound)
	class USoundCue* BreakSound;

	UPROPERTY(EditAnywhere, Category=Effect)
	class UParticleSystem* BreakEffect;

protected:
	virtual void OnFloorHit();

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
