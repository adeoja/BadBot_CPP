// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	float InterpSpd = 4;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DroneMesh;
};
