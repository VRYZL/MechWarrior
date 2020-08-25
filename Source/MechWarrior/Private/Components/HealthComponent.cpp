// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set defaults
	SetIsReplicatedByDefault(true);
	OwningActor = Cast<AMechBase>(GetOwner());
	ArmorMaxHealths.Init(100, 11);
	StructureMaxHealths.Init(100, 9);
	ArmorHealths = ArmorMaxHealths;
	StructureHealths = StructureMaxHealths;
}


void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, ArmorHealths);
	DOREPLIFETIME(UHealthComponent, StructureHealths);
	DOREPLIFETIME(UHealthComponent, ArmorMaxHealths);
	DOREPLIFETIME(UHealthComponent, StructureMaxHealths);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UHealthComponent::DamageArmor_Implementation(float Damage, EMechArmors ArmorPiece) {
	//If health of armor piece is greater than incoming damage, damage armor
	if (ArmorHealths[ArmorPiece] > Damage) {
		ArmorHealths[ArmorPiece] -= Damage;
	}
	else {
		EMechStructures part;
		switch (ArmorPiece)
		{
		case HeadArmor: part = Head;
			break;
		case LeftArmArmor: part = LeftArm;
			break;
		case LeftTorsoArmor: part = LeftTorso;
			break;
		case LeftTorsoRearArmor: part = LeftTorso;
			break;
		case CenterTorsoArmor: part = CenterTorso;
			break;
		case CenterTorsoRearArmor: part = CenterTorso;
			break;
		case RightTorsoArmor: part = RightTorso;
			break;
		case RightTorsoRearArmor: part = RightTorso;
			break;
		case RightArmArmor: part = RightArm;
			break;
		case LeftLegArmor: part = LeftLeg;
			break;
		case RightLegArmor: part = RightLeg;
			break;
		default:
			break;
		}
		//Destroy armor and carry over the remaining damage to underlying structure
		if (ArmorHealths[ArmorPiece] > 0) {
			OwningActor->MechArmorDestroyed(ArmorPiece);
			DamageStructure(Damage - ArmorHealths[ArmorPiece], part);
			ArmorHealths[ArmorPiece] = 0;
		}
		//No armor exists; Damage underlying structure
		else {
			DamageStructure(Damage, part);
		}
	}
	//Clamp the health value in range
	FMath::Clamp(ArmorHealths[ArmorPiece], 0.0f, ArmorMaxHealths[ArmorPiece]);
}


void UHealthComponent::DamageStructure_Implementation(float Damage, EMechStructures StructurePart) {
	//Apply damage based on the part damaged
	switch (StructurePart)
	{
	case Head:
		if (StructureHealths[Head] > Damage) {
			StructureHealths[Head] -= Damage;
		}
		else{
			StructureHealths[Head] = 0;
			OwningActor->MechPartDestroyed(Head);
			OwningActor->MechDestroyed();
		}
		break;
	case LeftArm:
		if (StructureHealths[LeftArm] > Damage) {
			StructureHealths[LeftArm] -= Damage;
		}
		else {
			if (StructureHealths[LeftArm] > 0) {
				OwningActor->MechPartDestroyed(LeftArm);
				DamageStructure(Damage - StructureHealths[LeftArm], LeftTorso);
				StructureHealths[LeftArm] = 0;
			}
			//Part does not exist, damage next part in hierarchy
			else {
				DamageStructure(Damage, LeftTorso);
			}
		}
		break;
	case LeftTorso:
		if (StructureHealths[LeftTorso] > Damage) {
			StructureHealths[LeftTorso] -= Damage;
		}
		else {
			if (StructureHealths[LeftTorso] > 0) {
				OwningActor->MechPartDestroyed(LeftTorso);
				DamageStructure(Damage - StructureHealths[LeftTorso], CenterTorso);
				StructureHealths[LeftTorso] = 0;
			}
			//Part does not exist, damage next part in hierarchy
			else {
				DamageStructure(Damage, CenterTorso);
			}
		}
		break;
	case CenterTorso:
		if (StructureHealths[CenterTorso] > Damage) {
			StructureHealths[CenterTorso] -= Damage;
		}
		else {
			StructureHealths[CenterTorso] = 0;
			OwningActor->MechPartDestroyed(CenterTorso);
			OwningActor->MechDestroyed();
		}
		break;
	case RightTorso:
		if (StructureHealths[RightTorso] > Damage) {
			StructureHealths[RightTorso] -= Damage;
		}
		else {
			if (StructureHealths[RightTorso] > 0) {
				OwningActor->MechPartDestroyed(RightTorso);
				DamageStructure(Damage - StructureHealths[RightTorso], CenterTorso);
				StructureHealths[RightTorso] = 0;
			}
			//Part does not exist, damage next part in hierarchy
			else {
				DamageStructure(Damage, CenterTorso);
			}
		}
		break;
	case RightArm:
		if (StructureHealths[RightArm] > Damage) {
			StructureHealths[RightArm] -= Damage;
		}
		else {
			if (StructureHealths[RightArm] > 0) {
				OwningActor->MechPartDestroyed(RightArm);
				DamageStructure(Damage - StructureHealths[RightArm], RightTorso);
				StructureHealths[RightArm] = 0;
			}
			//Part does not exist, damage next part in hierarchy
			else {
				DamageStructure(Damage, RightTorso);
			}
		}
		break;
	case LeftLeg:
		if (StructureHealths[LeftLeg] > Damage) {
			StructureHealths[LeftLeg] -= Damage;
		}
		else {
			if (StructureHealths[LeftLeg] > 0) {
				OwningActor->MechPartDestroyed(LeftLeg);
				DamageStructure(Damage - StructureHealths[LeftLeg], LeftTorso);
				StructureHealths[LeftLeg] = 0;
			}
			//Part does not exist, damage next part in hierarchy
			else {
				DamageStructure(Damage, LeftTorso);
			}
		}
		break;
	case RightLeg:
		if (StructureHealths[RightLeg] > Damage) {
			StructureHealths[RightLeg] -= Damage;
		}
		else {
			if (StructureHealths[RightLeg] > 0) {
				OwningActor->MechPartDestroyed(RightLeg);
				DamageStructure(Damage - StructureHealths[RightLeg], RightTorso);
				StructureHealths[RightLeg] = 0;
			}
			//Part does not exist, damage next part in hierarchy
			else {
				DamageStructure(Damage, RightTorso);
			}
		}
		break;
	case Engine:
		if (StructureHealths[Engine] > Damage) {
			StructureHealths[Engine] -= Damage;
		}
		else {
			StructureHealths[Engine] = 0;
			OwningActor->MechPartDestroyed(Engine);
			OwningActor->MechDestroyed();
		}
		break;
	default:
		break;
	}
	//Clamp the health value in range
	FMath::Clamp(StructureHealths[StructurePart], 0.0f, StructureMaxHealths[StructurePart]);
}


float UHealthComponent::GetArmorHealth(EMechArmors ArmorPiece) {
	return ArmorHealths[ArmorPiece];
}


float UHealthComponent::GetStructureHealth(EMechStructures StructurePart) {
	return StructureHealths[StructurePart];
}


void UHealthComponent::RepairAllDamages_Implementation() {
	ArmorHealths = ArmorMaxHealths;
	StructureHealths = StructureMaxHealths;
}


void UHealthComponent::RepairArmor_Implementation(float value, EMechArmors ArmorPiece) {
	ArmorHealths[ArmorPiece] += value;
	FMath::Clamp(ArmorHealths[ArmorPiece], 0.0f, ArmorMaxHealths[ArmorPiece]);
}


void UHealthComponent::RepairStructure_Implementation(float value, EMechStructures StructurePart) {
	StructureHealths[StructurePart] += value;
	FMath::Clamp(StructureHealths[StructurePart], 0.0f, StructureMaxHealths[StructurePart]);
}


void UHealthComponent::SetArmorMaxHealth_Implementation(float value, EMechArmors ArmorPiece) {
	ArmorMaxHealths[ArmorPiece] = value;
}


void UHealthComponent::SetStructureMaxHealth_Implementation(float value, EMechStructures StructurePart) {
	StructureMaxHealths[StructurePart] = value;
}


void UHealthComponent::SetAllArmorsMaxHealth_Implementation(const TArray<float> &Values) {
	if (Values.Num() == 11) {
		ArmorMaxHealths = Values;
	}
}


void UHealthComponent::SetAllStructuresMaxHealth_Implementation(const TArray<float> &Values) {
	if (Values.Num() == 9) {
		StructureMaxHealths = Values;
	}
}

