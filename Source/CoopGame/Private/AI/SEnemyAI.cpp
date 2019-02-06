// Fill out your copyright notice in the Description page of Project Settings.

#include "SEnemyAI.h"
#include "SCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Public/NavigationSystem.h"
#include "Public/NavigationPath.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "SHealthComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"


// Sets default values
ASEnemyAI::ASEnemyAI()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetCanEverAffectNavigation(false);
	SkeletalMeshComp->SetSimulatePhysics(true);
	RootComponent = SkeletalMeshComp;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASEnemyAI::HandleTakeDamage);

	/*SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(200);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(RootComponent);
*/
	bUseVelocityChange = false;
	MovementForce = 1000;
	RequiredDistanceToTarget = 100;
	ExplosionDamage = 40;
	ExplosionRadius = 1000;

	SelfDamageInterval = 0.25f;
}

// Called when the game starts or when spawned
void ASEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		NextPathPoint = GetNextPathPoint();

		FTimerHandle TimerHandle_CheckPowerLevel;
		GetWorldTimerManager().SetTimer(TimerHandle_CheckPowerLevel, this, &ASEnemyAI::OnCheckNearbyBots, 1.f, true);
	}
}
void ASEnemyAI::HandleTakeDamage(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (MatInst == nullptr)
	{
		MatInst = SkeletalMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, SkeletalMeshComp->GetMaterial(0));
	}
	if (MatInst)
	{
		MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);

	}
	if (Health <= 0.f)
	{
		SelfDestruct();
	}
}

FVector ASEnemyAI::GetNextPathPoint()
{
	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);

	if (NavPath->PathPoints.Num() > 1)
	{
		return NavPath->PathPoints[1];
	}

	return GetActorLocation();
}

void ASEnemyAI::SelfDestruct()

{
	if (bExploded)
	{
		return;
	}

	bExploded = true;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(this, ExplodeSound, GetActorLocation());

	SkeletalMeshComp->SetVisibility(false, true);
	SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Role == ROLE_Authority)
	{
		TArray <AActor*> IgnoredActors;
		IgnoredActors.Add(this);

		float ActualDamage = ExplosionDamage + (ExplosionDamage * PowerLevel);

		UGameplayStatics::ApplyRadialDamage(this, ActualDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

		DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 1.f, 0, 1.f);

		SetLifeSpan(2.0f);


	}

}

void ASEnemyAI::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!bStartedSelfDestruction && !bExploded)
	{
		ASCharacter* PlayerPawn = Cast<ASCharacter>(OtherActor);
		if (PlayerPawn)
		{
			if (Role == ROLE_Authority)
			{
				GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ASEnemyAI::DamageSelf, SelfDamageInterval, true, 1.0f);
			}


			bStartedSelfDestruction = false;

			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
		}

	}

}

void ASEnemyAI::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
}

void ASEnemyAI::OnCheckNearbyBots()
{
	const float Radius = 600.f;

	FCollisionShape CollShape;

	CollShape.SetSphere(Radius);

	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	QueryParams.AddObjectTypesToQuery(ECC_Pawn);


	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity, QueryParams, CollShape);

	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, FColor::Red, false, 1.0f);


	int32 NrOfBots = 0;
	for (FOverlapResult Result : Overlaps)
	{
		ASEnemyAI* Bot = Cast<ASEnemyAI>(Result.GetActor());
		if (Bot && Bot != this)
		{
			NrOfBots++;
		}

	}
	const int32 MaxPowerLevel = 8;

	PowerLevel = FMath::Clamp(NrOfBots, 0, MaxPowerLevel);

	if (MatInst == nullptr)
	{
		MatInst = SkeletalMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, SkeletalMeshComp->GetMaterial(0));
	}

	if (MatInst)
	{
		float Alpha = PowerLevel / (float)MaxPowerLevel;

		MatInst->SetScalarParameterValue("PowerLevelAlpha", Alpha);

	}
	DrawDebugString(GetWorld(), FVector(0, 0, 0), FString::FromInt(PowerLevel), this, FColor::White, 1.f, true);




}
// Called every frame
void ASEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Role == ROLE_Authority && !bExploded)
	{
		float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

		if (DistanceToTarget <= RequiredDistanceToTarget)
		{

			NextPathPoint = GetNextPathPoint();

			DrawDebugString(GetWorld(), GetActorLocation(), "TargetReached");
		}
		else
		{
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();

			ForceDirection *= MovementForce;

			SkeletalMeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);

			DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 32, FColor::Cyan, false, 0.f, 0, 1.f);
		}
		DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Cyan, false, 0.f, 1.f);

	}

}


//// Called to bind functionality to input
//void ASEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

