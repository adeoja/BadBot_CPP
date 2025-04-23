// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector CurrentLocation = GetActorLocation();
	TargetLocation = StartLocation + MoveDistance;
	
	FVector Destination;
	if (bReturn == false)
	{
		Destination = TargetLocation;
	}
	else
	{
		Destination = StartLocation;
	}
	
	FVector Movement = FMath::VInterpTo(CurrentLocation, Destination, DeltaTime, InterpSpd);
	SetActorLocation(Movement);

	if (FVector::Dist(CurrentLocation, Destination) <= 1)
	{
		bReturn = !bReturn;
	}
}

