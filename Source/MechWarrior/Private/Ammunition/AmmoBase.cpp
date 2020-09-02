// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammunition/AmmoBase.h"
#include "Mechs/MechBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AAmmoBase::AAmmoBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialise components
	AmmoVolume = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PickUpVolume"));
	AmmoMesh = CreateDefaultSubobject<UMeshComponent>(TEXT("PickUpMesh"));

	AmmoVolume->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AmmoVolume->SetGenerateOverlapEvents(true);
	AmmoVolume->SetHiddenInGame(true);
	AmmoVolume->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBase::OnHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileSystem"));
	ProjectileMovement->InitialSpeed = 0;
	ProjectileMovement->MaxSpeed = 0;
}


// Called when the game starts or when spawned
void AAmmoBase::BeginPlay()
{
	Super::BeginPlay();
	
	InitialLocation = K2_GetActorLocation();
}


// Called every frame
void AAmmoBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AAmmoBase::InitializeBullet(float MaxDamage, float AmmoOptimalRange, float AmmoMaxRange, float AmmoMinRange) {
	Damage = MaxDamage;
	OptimalRange = AmmoOptimalRange;
	MaxRange = AmmoMaxRange;
	MinRange = AmmoMinRange;
}


void AAmmoBase::InitializeProjectile(float InitialSpeed, float MaxSpeed, USceneComponent* HomingTarget) {
	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;
	if (HomingTarget != nullptr) {
		ProjectileMovement->bIsHomingProjectile = true;
		ProjectileMovement->HomingTargetComponent = HomingTarget;
	}
}


void AAmmoBase::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AMechBase* OtherMech = Cast<AMechBase>(OtherActor);
	if (OtherMech) {
		float DistanceTravelled = (GetActorLocation() - InitialLocation).Size();
		if (DistanceTravelled < MinRange) {
			//Bullet is not effective within this range, destroy it!
			Destroy();
		}
		else if (DistanceTravelled < OptimalRange) {
			//Bullet does full damage within this range!
			FDamageEvent Dummyevent;
			OtherComp->ReceiveComponentDamage(Damage, Dummyevent, GetInstigatorController(), this);
			Destroy();
		}
		else {
			//Bullet's effectiveness decreases linearly in this range
			float EffectiveDamage = Damage * (DistanceTravelled - MaxRange) / (OptimalRange - MaxRange);
			//Clamp effective damage to zero
			if (EffectiveDamage < 0) EffectiveDamage = 0;
			FDamageEvent Dummyevent;
			OtherComp->ReceiveComponentDamage(EffectiveDamage, Dummyevent, GetInstigatorController(), this);
			Destroy();
		}
	}
}