// Fill out your copyright notice in the Description page of Project Settings.

#include "SAICharacter.h"
#include "SWeapon.h"
#include "Components/SHealthComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"


// Sets default values
ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	
	
	//TODO ADDED TODAY
	WeaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComp"));

	WeaponAttachSocketName = "WeaponSocket";
}

// Called when the game starts or when spawned
void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
	}
	if (bDied)
	{
		bool DestroyActor(CurrentWeapon);
		SetLifeSpan(0.f);
	}
}

void ASAICharacter::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		bDied = true;
		//Die

		bool DestroyActor(CurrentWeapon);

		UE_LOG(LogTemp, Warning, TEXT("He died me!!!!"));

		SetLifeSpan(1.0f);
	}
	else
	{
		return;
	}
}
void ASAICharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();

	}
}

void ASAICharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}

}

// Called every frame
void ASAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

