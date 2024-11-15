// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock.h"

#include "MainCharacter.h"
#include "MainGameModeBase.h"
#include "Components/SphereComponent.h"

// Sets default values
ARock::ARock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Tags.Add("Rock");
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetSimulatePhysics(true);
	Sphere->SetNotifyRigidBodyCollision(true);
	Sphere->SetCollisionProfileName("PhysicsActor");
	Sphere->SetSphereRadius(50);
	RootComponent = Sphere;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Fab/boulder/boulder_a04.boulder_a04'"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetupAttachment(RootComponent);
	}
	Mesh->SetRelativeLocation(FVector(0, 0, -50));
	Mesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
}

// Called when the game starts or when spawned
void ARock::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentHit.AddDynamic(this, &ARock::OnHit);
}

// Called every frame
void ARock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARock::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->ActorHasTag(FName("Floor")))
		{
			OnFloorHit();
		}
		else if (OtherActor->ActorHasTag(FName("Player")))
		{
			Cast<AMainCharacter>(OtherActor)->TakeDamage(Damage);
		}
	}
}

void ARock::OnFloorHit()
{
	
}