// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AmmoComponent.h"
#include "Net/UnrealNetwork.h"
#include "Ammunition/AmmoBase.h"

// Sets default values for this component's properties
UAmmoComponent::UAmmoComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set defaults
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UAmmoComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


void UAmmoComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAmmoComponent, AmmoData);
}


// Called every frame
void UAmmoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UAmmoComponent::AddAmmoBin_Implementation(FAmmoData Ammo) {
	AmmoData.Add(Ammo);
}


void UAmmoComponent::RemoveAmmoBinAt_Implementation(uint8 index) {
	AmmoData.RemoveAt(index);
}


uint8 UAmmoComponent::GetNextBinIndexOfType(TSubclassOf<AAmmoBase> AmmoType, uint8 CurrentIndex) {
	for (uint8 i = CurrentIndex; i < AmmoData.Num(); i++) {
		//if type matches and there are ammo left, return the bin
		if (AmmoData[i].AmmoType == AmmoType && AmmoData[i].AmmoCount > 0) {
			return i;
		}
	}
	return -1;
}