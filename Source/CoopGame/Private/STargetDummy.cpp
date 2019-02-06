// Fill out your copyright notice in the Description page of Project Settings.

#include "STargetDummy.h"
#include "../Public/SCharacter.h"
#include "../Public/SWeapon.h"
#include "CoopGame.h"
#include "SHealthComponent.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASTargetDummy::ASTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
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

		bool DestroyActor(this);

		UE_LOG(LogTemp, Warning, TEXT("He died me!!!!"));

		SetLifeSpan(1.0f);
	}
	else
	{
		return;
	}
}
// Called every frame
void ASTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

