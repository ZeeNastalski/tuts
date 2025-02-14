// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"



// Sets default values
ASProjectile::ASProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASProjectile::OnHit);	// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;


	SetReplicates(true);
	SetReplicateMovement(true);

	InitialLifeSpan = .0f;
	
}




void ASProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	if (Role == ROLE_Authority)
	{		
		OnProjectileDestroyed();	
	}
}

void ASProjectile::OnProjectileDestroyed()
{
	if (!isDestroyed)
	{
		isDestroyed = true;


		DrawDebugSphere(GetWorld(), this->GetActorLocation(), ExplosionRadius, 32, FColor::Blue, false, 3.0f);


		if (ExplosionEffect)
		{

			TArray<AActor*> IgnoredActors;
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, this->GetActorLocation());
			UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, this->GetActorLocation(), ExplosionRadius, ExplosionDamageType, IgnoredActors);
		}

		Destroy();
	}
}

void ASProjectile::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle timeHandle;
	GetWorldTimerManager().SetTimer(timeHandle, this, &ASProjectile::OnProjectileDestroyed, 1.0f, false);
	
}

