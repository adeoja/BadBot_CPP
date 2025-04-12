#include "Kismet/KismetMathLibrary.h"
#include "GameFrameWork/PlayerController.h"
#include "CPP_Drone.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ACPP_Drone::ACPP_Drone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>("DroneMesh");
	RootComponent = DroneMesh;
}

// Called when the game starts or when spawned
void ACPP_Drone::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		DefaultPawn = PlayerController->GetPawn();  // Now GC-safe
	}
	
}

// Called every frame
void ACPP_Drone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(DefaultPawn))
	{
		FVector DroneLocation = GetActorLocation();
		FVector PawnLocation = DefaultPawn->GetActorLocation();
		FRotator DroneRotation = GetActorRotation();
		
		FRotator LookAtPawn = UKismetMathLibrary::FindLookAtRotation(DroneLocation, PawnLocation);
		FRotator LookAtPawnInterp = UKismetMathLibrary::RInterpTo(DroneRotation, LookAtPawn, DeltaTime, InterpSpd);
		SetActorRotation(LookAtPawnInterp);

		FVector L_RifleLoc = DroneMesh->GetSocketLocation("Rifle_L");
		DrawDebugSphere(GetWorld(), L_RifleLoc, 10, 12, FColor::Purple);

		FVector R_RifleLoc = DroneMesh->GetSocketLocation("Rifle_R");
		DrawDebugSphere(GetWorld(), R_RifleLoc, 10, 12, FColor::Red);
	}

}

