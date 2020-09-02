// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HeatComponent.h"
#include "Net/UnrealNetwork.h"
#include "Mechs/MechBase.h"

// Sets default values for this component's properties
UHeatComponent::UHeatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetIsReplicatedByDefault(true);
	OwningActor = Cast<AMechBase>(GetOwner());
}


void UHeatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHeatComponent, Temperature);
	DOREPLIFETIME(UHeatComponent, MaxT);
	DOREPLIFETIME(UHeatComponent, SinkTemperature);
	DOREPLIFETIME(UHeatComponent, IsCoolantOn);
	DOREPLIFETIME(UHeatComponent, CoolantAmount);
	DOREPLIFETIME(UHeatComponent, MaxCoolantAmount);
	DOREPLIFETIME(UHeatComponent, NumSHS);
	DOREPLIFETIME(UHeatComponent, NumDHS);
}


// Called when the game starts
void UHeatComponent::BeginPlay()
{
	Super::BeginPlay();

	//Set Important variables.
	CalculatePrimaryVariables();
	if (NumDHS > 0) {
		NumSHS = 0;
	}
	
}


// Called every frame
void UHeatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Calculate heat every frame
	CalculateHeat(DeltaTime);
}


void UHeatComponent::CalculatePrimaryVariables() {
	//Calculate k_mod with coolant
	EHS = NumSHS + Ext_Multiplier * (NumDHS - 10) + CoolantFactor * (10);
	HS_Mult = hsmLB + (hsmUB - hsmLB) / (44 - 10) * (EHS - 10);
	Mass_Mult = mmLB + (mmUB - mmLB) / (100 - 20) * (OwningActor->Tonnage - 20);
	k_mod_WC = (k + (EHS * k_c1) + (k_c2 / EHS)) * HS_Mult * Mass_Mult * GlobalConductivity;

	//Calculate k_mod without coolant
	EHS = NumSHS + Ext_Multiplier * (NumDHS - 10) + Int_Multiplier * (10);
	MaxT = 30 + EHS;
	HS_Mult = hsmLB + (hsmUB - hsmLB) / (44 - 10) * (EHS - 10);
	k_mod = (k + (EHS * k_c1) + (k_c2 / EHS)) * HS_Mult * Mass_Mult * GlobalConductivity;
}


void UHeatComponent::AddHeat_Implementation(float DeltaHeat, float OverTime) {
	//Add heat input instantaneously
	if (OverTime == 0)
		Temperature += DeltaHeat;
	//Save data to add heat overtime
	else {
		HeatData temp;
		temp.HeatVal = DeltaHeat;
		temp.duration = OverTime;
		temp.timeRemaining = OverTime;
		HeatDataArray.Add(temp);
	}
}


void UHeatComponent::CalculateHeat(float DeltaSeconds) {
	//Accumulate input heat every frame
	for (int i = 0; i < HeatDataArray.Num(); i++) {
		if (HeatDataArray[i].timeRemaining > DeltaSeconds) {
			Temperature += (HeatDataArray[i].HeatVal / HeatDataArray[i].duration) * DeltaSeconds;
			HeatDataArray[i].timeRemaining -= DeltaSeconds;
		}
		else {
			Temperature += (HeatDataArray[i].HeatVal / HeatDataArray[i].duration) * HeatDataArray[i].timeRemaining;
			HeatDataArray.RemoveAt(i);
			i--;
		}
	}

	//Dissipate heat every frame
	if (Temperature < 1.01 * SinkTemperature) {
		Temperature = SinkTemperature;
	}
	else {
		if (IsCoolantOn) {
			//If is running low on coolant, turn off coolant flow
			if (CoolantAmount < CoolantConsumptionRate * DeltaSeconds) {
				IsCoolantOn = false;
			}
			else {
				Q = (k_mod_WC * A * (Temperature - SinkTemperature) / d) * (DeltaSeconds);
				CoolantAmount -= CoolantConsumptionRate * DeltaSeconds;
			}
		}
		else {
			Q = (k_mod * A * (Temperature - SinkTemperature) / d) * (DeltaSeconds);
		}

		Temperature -= Q * 0.000526565076466;
	}

	//Update HeatStatus and call neccessary events
	if (MechHeatStatus == Cool) {
		if (Temperature > HotThreshold * MaxT) {
			MechHeatStatus = Heated;
			OwningActor->Heated();
		}
		else if (Temperature > OverHeatedThreshold * MaxT) {
			MechHeatStatus = OverHeated;
			OwningActor->OverHeated();
		}
	}
	else if (MechHeatStatus == Heated) {
		if (Temperature < HotThreshold * MaxT) {
			MechHeatStatus = Cool;
			OwningActor->CooledDown();
		}
		else if (Temperature > OverHeatedThreshold * MaxT) {
			MechHeatStatus = OverHeated;
			OwningActor->OverHeated();
		}
	}
	else {
		if (Temperature < HotThreshold * MaxT) {
			MechHeatStatus = Cool;
			OwningActor->CooledDown();
		}
		else if (Temperature < OverHeatedThreshold * MaxT) {
			MechHeatStatus = Heated;
			OwningActor->Heated();
		}
	}
}


void UHeatComponent::EnableCoolant_Implementation(bool enable) {
	if (enable && CoolantAmount > CoolantConsumptionRate) {
		IsCoolantOn = true;
	}
	else IsCoolantOn = false;
}


void UHeatComponent::RefillCoolant_Implementation() {
	CoolantAmount = MaxCoolantAmount;
}


void UHeatComponent::SetHeatSinks_Implementation(HeatSinkTypes HeatSinkType, int32 NumHeatSinks) {

	if (HeatSinkType == SingleHeatSink) {
		NumSHS = NumHeatSinks;
		NumDHS = 0;
	}
	else {
		NumDHS = NumHeatSinks;
		NumSHS = 0;
	}
	CalculatePrimaryVariables();
}

