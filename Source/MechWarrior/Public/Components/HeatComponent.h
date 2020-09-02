// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeatComponent.generated.h"

UENUM(BlueprintType)
enum HeatSinkTypes {
	SingleHeatSink, DoubleHeatSink
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHWARRIOR_API UHeatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHeatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Variables for local calculations
	struct HeatData {
		float HeatVal;
		float duration;
		float timeRemaining;
	};

	enum HeatLevels {
		Cool, Heated, OverHeated
	};

	TArray<HeatData> HeatDataArray;
	HeatLevels MechHeatStatus = Cool;
	class AMechBase* OwningActor;

	//Replicated variables
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "HeatSettings")
		float Temperature = 10;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "HeatSettings")
		float SinkTemperature=0;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "HeatSettings")
		float MaxT;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "HeatSettings")
		bool IsCoolantOn = false;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "HeatSettings")
		int32 NumSHS;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "HeatSettings")
		int32 NumDHS = 10;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "HeatSettings")
		float CoolantAmount = 10;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "HeatSettings")
		float MaxCoolantAmount = 10;

	//HeatSettings
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HeatSettings")
		float HotThreshold = 0.7;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HeatSettings")
		float OverHeatedThreshold = 1;

	//Constants
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants")
		float A = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants")
		float d = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants")
		float k = 50.2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants")
		float k_c1 = 25;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants")
		float k_c2 = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants")
		float Ext_Multiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants")
		float Int_Multiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants")
		float CoolantFactor = 3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants")
		float CoolantConsumptionRate = .5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants")
		float hsmUB = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants")
		float hsmLB = 0.7;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants")
		float mmUB = 1.2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants")
		float mmLB = 0.9;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Constants")
		float GlobalConductivity = 0.75;

	//Functions
	void CalculateHeat(float DeltaSeconds);
	void CalculatePrimaryVariables();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Heat")
	void AddHeat(float DeltaHeat, float OverTime);
	void AddHeat_Implementation(float DeltaHeat, float OverTime);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Heat")
	void EnableCoolant(bool enable);
	void EnableCoolant_Implementation(bool enable);

	UFUNCTION(Server, UnReliable, BlueprintCallable, Category = "Heat")
	void RefillCoolant();
	void RefillCoolant_Implementation();

	UFUNCTION(Server, UnReliable, BlueprintCallable, Category = "Heat")
	void SetHeatSinks(HeatSinkTypes HeatSinkType, int32 NumHeatSinks);
	void SetHeatSinks_Implementation(HeatSinkTypes HeatSinkType, int32 NumHeatSinks);

	//Overriding parent implementations
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

private:
	float Q;
	float HS_Mult, Mass_Mult, EHS, k_mod_WC, k_mod;
		
};
