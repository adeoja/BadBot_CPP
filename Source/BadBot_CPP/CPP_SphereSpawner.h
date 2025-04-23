// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_SmallSphere.h"
#include "GameFramework/Actor.h"
#include "CPP_SphereSpawner.generated.h"

UCLASS()
class BADBOT_CPP_API ACPP_SphereSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_SphereSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	float SpawnRadius = 250;

	UPROPERTY(EditAnywhere)
	float SpawnTime = 3;

	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACPP_SmallSphere> SmallSphere;

	UFUNCTION()
	void SpawnSphere();
};
