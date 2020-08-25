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
	UPROPERTY(Replicated, BlueprintReadWrite)
	TArray<float> ArmorHealths;
	UPROPERTY(Replicated, BlueprintReadWrite)
	TArray<float> StructureHealths;
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite)
	TArray<float> ArmorMaxHealths;
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite)
	TArray<float> StructureMaxHealths;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AMechBase* OwningActor;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Overriding parent implementations
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	//Functions
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void DamageArmor(float Damage, EMechArmors ArmorPiece);
	void DamageArmor_Implementation(float Damage, EMechArmors ArmorPiece);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void DamageStructure(float Damage, EMechStructures StructurePart);
	void DamageStructure_Implementation(float Damage, EMechStructures StructurePart);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void RepairAllDamages();
	void RepairAllDamages_Implementation();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void RepairArmor(float value, EMechArmors ArmorPiece);
	void RepairArmor_Implementation(float value, EMechArmors ArmorPiece);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void RepairStructure(float value, EMechStructures StructurePart);
	void RepairStructure_Implementation(float value, EMechStructures StructurePart);

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
