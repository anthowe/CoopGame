// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"


class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Weapon")
	TSubclassOf<UDamageType> DamageType;




	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	void PlayFireEffects(FVector TraceEnd);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf <UCameraShake> FireCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon")
	TSubclassOf<AActor> BulletClass;

	virtual void Fire();

	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFireTime;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	float TimeBetweenShots;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();
	
	//Test code below
	/*UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	*/
};
