#include "CPP_Drone.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFrameWork/PlayerController.h"

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
		DefaultPawn = PlayerController->GetPawn();  
	}

	// Delay before firing blasters
	GetWorld()->GetTimerManager().SetTimer(Delay, this, &ACPP_Drone::FireBlasters, 3.0f, false);
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


void ACPP_Drone::FireBlasters() 
{
	if (DroneMesh && DroneMesh->DoesSocketExist("Rifle_L") && DroneMesh->DoesSocketExist("Rifle_R") && L_BlasterBeam && R_BlasterBeam)
	{
		FVector L_RifleLocation = DroneMesh->GetSocketLocation("Rifle_L");
		FRotator L_RifleRotation = DroneMesh->GetSocketRotation("Rifle_L");
		GetWorld()->SpawnActor<ACPP_BlasterBeam>(L_BlasterBeam, L_RifleLocation, L_RifleRotation);
		if (BlastEffectAsset)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BlastEffectAsset, L_RifleLocation);
		}
    
		FVector R_RifleLocation = DroneMesh->GetSocketLocation("Rifle_R");
		FRotator R_RifleRotation = DroneMesh->GetSocketRotation("Rifle_R");
		GetWorld()->SpawnActor<ACPP_BlasterBeam>(R_BlasterBeam, R_RifleLocation, R_RifleRotation);
		if (BlastEffectAsset)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BlastEffectAsset, R_RifleLocation);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Blaster not firing"));
	}
}


