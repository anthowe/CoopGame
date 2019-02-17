// Fill out your copyright notice in the Description page of Project Settings.

#include "SPickupActor.h"
#include "SPowerupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"


// Sets default values
ASPickupActor::ASPickupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.f);
	RootComponent = SphereComp;


	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	DecalComp->DecalSize=(FVector(64.f, 75.f, 75.f));
	DecalComp->SetupAttachment(RootComponent);

	CooldownDuration = 10.0f;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		Respawn();
	}

	
}

void ASPickupActor::Respawn()
{
	if (PowerupClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Powerup class is nullptr in %s.  Please update your Blueprint"), *GetName());
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PowerupInstance = GetWorld()->SpawnActor<ASPowerupActor>(PowerupClass, GetTransform(), SpawnParams);
}

void ASPickupActor::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Role == ROLE_Authority && PowerupInstance)
	{
		PowerupInstance->ActivatePowerup(OtherActor);
		PowerupInstance = nullptr;
	}

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPickupActor::Respawn, CooldownDuration);
}


