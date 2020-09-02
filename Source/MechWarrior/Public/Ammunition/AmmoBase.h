// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoBase.generated.h"

UCLASS()
class MECHWARRIOR_API AAmmoBase : public AActor
{
	GENERATED_BODY()

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUpVolume, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* AmmoVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUpMesh, meta = (AllowPrivateAccess = "true"))
	class UMeshComponent* AmmoMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUpMesh, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
	
public:	
	// Sets default values for this actor's properties
	AAmmoBase();

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float Damage = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float OptimalRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float MaxRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float MinRange;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Internal variables
	FVector InitialLocation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void InitializeBullet(float MaxDamage, float AmmoOptimalRange, float AmmoMaxRange, float AmmoMinRange = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void InitializeProjectile(float InitaialSpeed, float MaxSpeed, class USceneComponent* HomingTarget = nullptr);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
