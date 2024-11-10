// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rock.h"
#include "Rock1.generated.h"

UCLASS()
class CUMPUTERGAMEDESIGN_API ARock1 : public ARock
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARock1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnFloorHit() override;
	
};
