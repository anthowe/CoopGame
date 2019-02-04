// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/SCharacter.h"
#include "../Public/SWeapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoopGame.h"
#include "SWeapon.h"
#include "SHealthComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASTargetDummy::ASTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
}

// Called when the game starts or when spawned
void ASTargetDummy::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);

	
}
void ASTargetDummy::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	/*if (DefaultHealth >= 0)
	{
		float CurrentHealth = DefaultHealth - HealthDelta;
		UE_LOG(LogTemp, Warning, TEXT("Current Health: %f"), CurrentHealth)
	}*/


	if (Health <= 0.0f && !bDied)
	{
		bDied = true;
		//Die

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(10.0f);
	}
}
// Called every frame
void ASTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

