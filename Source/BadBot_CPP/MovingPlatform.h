// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class BADBOT_CPP_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector TargetLocation;
	
	UPROPERTY(EditAnywhere)
	FVector MoveDistance = FVector(0,0,0);

	UPROPERTY(EditAnywhere)
	float InterpSpd = 4;

	UPROPERTY()
	bool bReturn = false;
};
