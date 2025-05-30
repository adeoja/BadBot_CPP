// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "CPP_BlasterBeam.h"
#include "CPP_Drone.generated.h"

UCLASS()
class BADBOT_CPP_API ACPP_Drone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Drone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	APawn* DefaultPawn;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DroneMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACPP_BlasterBeam> L_BlasterBeam;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACPP_BlasterBeam> R_BlasterBeam;

	UPROPERTY(EditAnywhere)
	float InterpSpd = 4;

	UPROPERTY(EditAnywhere, Category = "Timer")
	float DelayStart = 0;

	UPROPERTY(EditAnywhere, Category = "Timer")
	float DelayBlast = .2;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* BlastEffectAsset;

	UFUNCTION()
	void FireBlasters();
	
	UFUNCTION()
	void Spawn_L_Blaster();

	UFUNCTION()
	void Spawn_R_Blaster();

	UFUNCTION()
	void DelayTimer();

	FTimerHandle BlastDelay;
	FTimerHandle StartDelay;

	UPROPERTY(EditAnywhere)
	USoundBase* BlasterSound;

	UPROPERTY()
	bool SwitchRifle = false;

	UPROPERTY(EditAnywhere)
	int MoveSpeed = 100;
};
