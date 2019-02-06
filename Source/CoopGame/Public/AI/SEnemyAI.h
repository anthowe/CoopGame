// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SEnemyAI.generated.h"

class USHealthComponent;
class USphereComponent;
class USoundCue;
class USkeletalMeshComponent;


UCLASS()
class COOPGAME_API ASEnemyAI : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASEnemyAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		USHealthComponent* HealthComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USoundCue* SelfDestructSound;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USoundCue* ExplodeSound;

	UFUNCTION()
		void HandleTakeDamage(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,
			class AController* InstigatedBy, AActor* DamageCauser);

	FVector GetNextPathPoint();

	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		float RequiredDistanceToTarget;

	//dynamic material to pulse on damage
	UMaterialInstanceDynamic* MatInst;

	void SelfDestruct();

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		UParticleSystem* ExplosionEffect;

	bool bExploded;

	bool bStartedSelfDestruction;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		float ExplosionDamage;

	FTimerHandle TimerHandle_SelfDamage;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		float SelfDamageInterval;

	void DamageSelf();

	void OnCheckNearbyBots();

	int32 PowerLevel;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
};
