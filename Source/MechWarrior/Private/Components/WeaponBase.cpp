// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponBase.h"
#include "Net/UnrealNetwork.h"
#include "Mechs/MechBase.h"
#include "Components/HeatComponent.h"
#include "Ammunition/AmmoBase.h"

// Sets default values for this component's properties
UWeaponBase::UWeaponBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set defaults
	SetIsReplicatedByDefault(true);
	OwningActor = Cast<AMechBase>(GetOwner());
}


void UWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponBase, Health);
	DOREPLIFETIME(UWeaponBase, Tons);
	DOREPLIFETIME(UWeaponBase, CurrentBinIndex);
}


// Called when the game starts
void UWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AmmoManager = OwningActor->AmmoSystem;
	TimeSinceLastShot = CoolDownDuration;

}


// Called every frame
void UWeaponBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update Variables
	TimeSinceLastShot += DeltaTime;
}


void UWeaponBase::Fire_Implementation() {
	//Shoot if cooldown is over
	if (TimeSinceLastShot > CoolDownDuration && AmmoManager->AmmoData[CurrentBinIndex].AmmoCount > 0) {
		RemainingShots = AmmoManager->AmmoData[CurrentBinIndex].ShotsPerRound;
		//If AmmoCount is less than number of shots in a round, set num shots to ammo count
		if (RemainingShots > AmmoManager->AmmoData[CurrentBinIndex].AmmoCount) {
			RemainingShots = AmmoManager->AmmoData[CurrentBinIndex].AmmoCount;
		}
		AmmoManager->AmmoData[CurrentBinIndex].AmmoCount -= RemainingShots;
		//Spawn bullets after SpoolDuration
		FTimerHandle UnusedHandle;
		if (SpoolDuration == 0) {
			SpawnProjectile();
		}
		else {
			GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &UWeaponBase::SpawnProjectile, SpoolDuration, false);
		}
		TimeSinceLastShot = 0;
		//If ammo bin is empty, get next usable ammo bin
		if (AmmoManager->AmmoData[CurrentBinIndex].AmmoCount == 0) {
			for (TSubclassOf<AAmmoBase> Type : SupportedAmmoTypes) {
				uint8 NextIndex = AmmoManager->GetNextBinIndexOfType(Type, CurrentBinIndex);
				if (NextIndex != -1) {
					CurrentBinIndex = NextIndex;
					break;
				}
			}
		}
	}
}


void UWeaponBase::SpawnProjectile() {
	//The actual spawning
	AAmmoBase* Ammo = GetWorld()->SpawnActorDeferred<AAmmoBase>(AmmoManager->AmmoData[CurrentBinIndex].AmmoType, GetSocketTransform(MuzzleSocketName));
	Ammo->InitializeBullet(AmmoManager->AmmoData[CurrentBinIndex].DamageValue, OptimalRange, MaxRange, MinRange);
	Ammo->InitializeProjectile(LaunchVelocity, LaunchVelocity);
	Ammo->FinishSpawning(GetSocketTransform(MuzzleSocketName));
	//Update Mech
	RemainingShots -= 1;
	OwningActor->HeatSystem->AddHeat(HeatGeneratedPerShot, 0.0f);
	//Prepare for next shots if any
	if (RemainingShots > 0) {
		if (TimeBeforeNextShot == 0) {
			SpawnProjectile();
		}
		else {
			FTimerHandle UnusedHandle;
			GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &UWeaponBase::SpawnProjectile, TimeBeforeNextShot, false);
		}
	}
}


UAmmoComponent* UWeaponBase::GetAmmoManager() {
	return AmmoManager;
}


void UWeaponBase::SwitchAmmoBin_Implementation(uint8 index) {
	if (AmmoManager->AmmoData.Num() <= index) {
		CurrentBinIndex = index;
	}
}


uint8 UWeaponBase::GetCurrentBinIndex() {
	return CurrentBinIndex;
}

