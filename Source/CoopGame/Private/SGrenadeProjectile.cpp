// Fill out your copyright notice in the Description page of Project Settings.

#include "SGrenadeProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine.h"
#include "Components/ActorComponent.h"

// Sets default values
ASGrenadeProjectile::ASGrenadeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	
	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";

}

// Called when the game starts or when spawned
void ASGrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASGrenadeProjectile::ExplodeProjectile()
{
	UE_LOG(LogTemp, Warning, TEXT("DONKEY: Grenade Fire"));

	AActor*MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();

		FVector TraceEnd = EyeLocation + ShotDirection;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.bTraceComplex = true;

		FVector TracerEndPoint = TraceEnd;

		FHitResult Hit;

		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			AActor* HitActor = Hit.GetActor();
			UGameplayStatics::ApplyRadialDamage(GetWorld(), 100.0f, EyeLocation, 250.f, DamageType, TArray<AActor*>(), this, MyOwner->GetInstigatorController());
			PlayExplosionEffect(TraceEnd);
		}

		
	}
	
}
void ASGrenadeProjectile::PlayExplosionEffect(FVector TraceEnd)
{
	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(ExplosionEffect, MeshComp, TracerTargetName);
	}

	//if (TracerEffect)
	//{
	//	FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
	//	UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
	//	if (TracerComp)
	//	{
	//		TracerComp->SetVectorParameter(TracerTargetName, TraceEnd);
	//	}
	//}
}



// Called every frame
void ASGrenadeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

