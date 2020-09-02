// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MechBase.generated.h"

UENUM(BlueprintType)
enum EMechArmors {
	HeadArmor, LeftArmArmor, LeftTorsoArmor, LeftTorsoRearArmor, CenterTorsoArmor, CenterTorsoRearArmor,
	RightTorsoArmor, RightTorsoRearArmor, RightArmArmor, LeftLegArmor, RightLegArmor
};

UENUM(BlueprintType)
enum EMechStructures {
	Head, LeftArm, LeftTorso, CenterTorso, RightTorso, RightArm, LeftLeg, RightLeg, Engine
};

UCLASS()
class MECHWARRIOR_API AMechBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMechBase();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	//Mech Characteristics
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MechCharacteristics", Meta = (ClampMin = 20, ClampMax = 100, UIMin = 20, UIMax = 100))
	float Tonnage = 60;

	//Components
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Heat, meta = (AllowPrivateAccess = "true"))
		class UHeatComponent* HeatSystem;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		class UHealthComponent* HealthSystem;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
		class UAmmoComponent* AmmoSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Inputs
	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Set replicated props
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	//Functions
	UFUNCTION(BlueprintNativeEvent, Category = "Mech Events")
	void Heated();
	void Heated_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Mech Events")
	void OverHeated();
	void OverHeated_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Mech Events")
	void CooledDown();
	void CooledDown_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Mech Events")
	void OnMechPartDestroyed(EMechStructures DestoyedPart);
	void OnMechPartDestroyed_Implementation(EMechStructures DestoyedPart);

	UFUNCTION(BlueprintNativeEvent, Category = "Mech Events")
	void OnMechArmorDestroyed(EMechArmors DestoyedArmor);
	void OnMechArmorDestroyed_Implementation(EMechArmors DestoyedArmor);

	UFUNCTION(BlueprintNativeEvent, Category = "Mech Events")
	void OnMechDestroyed();
	void OnMechDestroyed_Implementation();

	//Telecast to all players about the destruction events
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Mech Events")
	void MechPartDestroyed(EMechStructures DestoyedPart);
	void MechPartDestroyed_Implementation(EMechStructures DestoyedPart);

	UFUNCTION(NetMulticast, Reliable, Category = "Mech Events")
	void MechArmorDestroyed(EMechArmors DestoyedArmor);
	void MechArmorDestroyed_Implementation(EMechArmors DestoyedArmor);

	UFUNCTION(NetMulticast, Reliable, Category = "Mech Events")
	void MechDestroyed();
	void MechDestroyed_Implementation();


};
