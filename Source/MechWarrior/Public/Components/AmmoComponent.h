// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AmmoComponent.generated.h"

USTRUCT(BlueprintType)
struct FAmmoData {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AmmoData")
	class TSubclassOf<class AAmmoBase> AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AmmoData")
	uint8 AmmoCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AmmoData")
	uint8 ShotsPerRound = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AmmoData")
	float DamageValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AmmoData")
	float InternalExplosionProbability = 0.1f;

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MECHWARRIOR_API UAmmoComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAmmoComponent();

	//Replicated Variables
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	TArray<FAmmoData> AmmoData;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Protected Replicated Variables


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Set replicated props
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	//internal functions


	//Functions
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "AmmoSystem")
	uint8 GetNextBinIndexOfType(TSubclassOf<class AAmmoBase> AmmoType, uint8 CurrentIndex = 0);

	//Server only Functions
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "AmmoSystem")
	void AddAmmoBin(FAmmoData Ammo);
	void AddAmmoBin_Implementation(FAmmoData Ammo);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "AmmoSystem")
	void RemoveAmmoBinAt(uint8 index);
	void RemoveAmmoBinAt_Implementation(uint8 index);

};
