// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AmmoComponent.h"
#include "WeaponBase.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MECHWARRIOR_API UWeaponBase : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UWeaponBase();

	//Replicated Variables
	UPROPERTY(Replicated, EditAnyWhere, BlueprintReadWrite, Category = "Weapon Stats")
	float Health = 100;
	UPROPERTY(Replicated, EditAnyWhere, BlueprintReadWrite, Category = "Weapon Stats")
	float Tons = 1;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	uint8 CurrentBinIndex = 0;
	
	//Variables
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Ammo")
	FName MuzzleSocketName = "Muzzle";
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Ammo")
	TArray<TSubclassOf<class AAmmoBase>> SupportedAmmoTypes;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Weapon Stats")
	float OptimalRange;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Weapon Stats")
	float MaxRange;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Weapon Stats")
	float MinRange = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Weapon Stats")
	float CoolDownDuration = 3;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Weapon Stats")
	float HeatGeneratedPerShot;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Weapon Stats")
	uint8 NumSlots;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Weapon Stats")
	float LaunchVelocity;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Weapon Stats")
	float HitImpulse;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Weapon Stats")
	float InternalExplotionProbability = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Weapon Stats")
	float JamDuration;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Weapon Stats")
	float SpoolDuration = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Weapon Stats")
	float TimeBeforeNextShot = 0.1f;

	//Miscellaneous Variables
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Miscellaneous")
	float TimeSinceLastShot;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Miscellaneous")
	uint8 RemainingShots = 0;

	//Internal variables
	class AMechBase* OwningActor;
	class UAmmoComponent* AmmoManager;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Set replicated props
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;
	
	//Internal functions
	void SpawnProjectile();

	//Functions
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "AmmoSystem")
	class UAmmoComponent* GetAmmoManager();
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "AmmoSystem")
	uint8 GetCurrentBinIndex();

	//Server only Functions
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Weapons")
	void Fire();
	void Fire_Implementation();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "AmmoSystem")
	void SwitchAmmoBin(uint8 index);
	void SwitchAmmoBin_Implementation(uint8 index);

};
