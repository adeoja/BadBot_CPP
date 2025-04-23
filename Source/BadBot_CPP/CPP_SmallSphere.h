// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "CPP_BadBotGamemode.h"
#include "GameFramework/Actor.h"
#include "CPP_SmallSphere.generated.h"

UCLASS()
class BADBOT_CPP_API ACPP_SmallSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_SmallSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	float SphereSpeed;

	UPROPERTY()
	FRotator SphereRotation;
	
	FTimerHandle WinkOutTimer;

	UFUNCTION()
	void WinkOut();

	UPROPERTY()
	bool bStartScaling = false;

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComp;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
					bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	APawn* DefaultPawn;

	UPROPERTY()
	ACPP_BadBotGamemode* BadBotGameMode;
	
	UPROPERTY()
	bool IsWinking = false;
};
