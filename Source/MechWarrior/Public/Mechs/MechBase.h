// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MechBase.generated.h"

UCLASS()
class MECHWARRIOR_API AMechBase : public ACharacter
{
	GENERATED_BODY()

	//Components
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Heat, meta = (AllowPrivateAccess = "true"))
	class UHeatComponent* Heat;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Heat, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* Health;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Heat, meta = (AllowPrivateAccess = "true"))
	class UAmmoComponent* Ammo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

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

};
