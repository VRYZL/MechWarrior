// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mechs/MechBase.h"
#include "HealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHWARRIOR_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	//Variables
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "HealthProperties")
	TArray<float> ArmorHealths;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "HealthProperties")
	TArray<float> StructureHealths;
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "HealthProperties")
	TArray<float> ArmorMaxHealths;
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "HealthProperties")
	TArray<float> StructureMaxHealths;
	UPROPERTY(EditDefaultsOnly, Category = "HealthProperties", meta = (ClampMin = 0, UIMin = 0))
	float RepairInterruptDuration = 2.0f;

	//for internal calculation
	struct StructRepairData {
		float RepairRate;
		float* Part;
		float Value;
		float ValueRemaining;
		float MaxValue;
	};

protected:
	//Internal variables
	float GlobalRepairRate = 10.0f;
	bool IsBeingRepaired = false;
	bool IsRepairInturrupted = false;
	float RepairStandByTime = 0;
	//TArray<StructRepairData> RepairData;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AMechBase* OwningActor;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Overriding parent implementations
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	//Internal functions
	//void MakeRepairs(float DeltaTime);
	void RepairAll(float DeltaTime);
	void InturruptRepairs();

	//UFunctions
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void DamageArmor(float Damage, EMechArmors ArmorPiece);
	void DamageArmor_Implementation(float Damage, EMechArmors ArmorPiece);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void DamageStructure(float Damage, EMechStructures StructurePart);
	void DamageStructure_Implementation(float Damage, EMechStructures StructurePart);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void RepairAllDamages(float RepairRate = 10.0f);
	void RepairAllDamages_Implementation(float RepairRate = 10.0f);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void StopRepairs();
	void StopRepairs_Implementation();

	/*UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void RepairAllParts(float RepairRate = 10.0f, float RepairValue = 50);
	void RepairAllParts_Implementation(float RepairRate = 10.0f, float RepairValue = 50);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void RepairArmor(float value, EMechArmors ArmorPiece, float RepairRate = 10.0f);
	void RepairArmor_Implementation(float value, EMechArmors ArmorPiece, float RepairRate = 10.0f);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void RepairStructure(float value, EMechStructures StructurePart, float RepairRate = 10.0f);
	void RepairStructure_Implementation(float value, EMechStructures StructurePart, float RepairRate = 10.0f);*/

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Health")
	float GetArmorHealth(EMechArmors ArmorPiece);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Health")
	float GetStructureHealth(EMechStructures StructurePart);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void SetArmorMaxHealth(float value, EMechArmors ArmorPiece);
	void SetArmorMaxHealth_Implementation(float value, EMechArmors ArmorPiece);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void SetStructureMaxHealth(float value, EMechStructures StructurePart);
	void SetStructureMaxHealth_Implementation(float value, EMechStructures StructurePart);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void SetAllArmorsMaxHealth(const TArray<float> &Values);
	void SetAllArmorsMaxHealth_Implementation(const TArray<float> &Values);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void SetAllStructuresMaxHealth(const TArray<float> &Values);
	void SetAllStructuresMaxHealth_Implementation(const TArray<float> &Values);
};
