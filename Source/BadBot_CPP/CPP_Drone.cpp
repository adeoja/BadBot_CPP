#include "CPP_Drone.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
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

	GetWorld()->GetTimerManager().SetTimer(StartDelay, this, &ACPP_Drone::DelayTimer, DelayStart, false);
	
	
}

// Called every frame
void ACPP_Drone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(DefaultPawn))
	{
		FVector DroneLocation = GetActorLocation();
		FRotator DroneRotation = GetActorRotation();
		
		FVector PawnLocation = DefaultPawn->GetActorLocation();
		
		FRotator LookAtPawn = UKismetMathLibrary::FindLookAtRotation(DroneLocation, PawnLocation);
		FRotator LookAtPawnInterp = UKismetMathLibrary::RInterpTo(DroneRotation, LookAtPawn, DeltaTime, InterpSpd);
		SetActorRotation(LookAtPawnInterp);

		FVector MoveLocation = GetActorForwardVector() * DeltaTime * MoveSpeed;
		AddActorWorldOffset(MoveLocation);

		FVector L_RifleLoc = DroneMesh->GetSocketLocation("Rifle_L");
		FVector R_RifleLoc = DroneMesh->GetSocketLocation("Rifle_R");
	}
}


void ACPP_Drone::FireBlasters() 
{
	if (DroneMesh && DroneMesh->DoesSocketExist("Rifle_L") && DroneMesh->DoesSocketExist("Rifle_R") && L_BlasterBeam && R_BlasterBeam && BlasterSound && BlastEffectAsset)
	{
		if (SwitchRifle)
		{
			Spawn_L_Blaster();
		}
		else
		{
			Spawn_R_Blaster();
		}
		SwitchRifle = !SwitchRifle;
	}
	
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Blaster not firing"));
	}
}

void ACPP_Drone::Spawn_L_Blaster()
{
	FVector L_RifleLocation = DroneMesh->GetSocketLocation("Rifle_L");
	FRotator L_RifleRotation = DroneMesh->GetSocketRotation("Rifle_L");
	GetWorld()->SpawnActor<ACPP_BlasterBeam>(L_BlasterBeam, L_RifleLocation, L_RifleRotation);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BlastEffectAsset, L_RifleLocation);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), BlasterSound, L_RifleLocation);
}

void ACPP_Drone::Spawn_R_Blaster()
{
	FVector R_RifleLocation = DroneMesh->GetSocketLocation("Rifle_R");
	FRotator R_RifleRotation = DroneMesh->GetSocketRotation("Rifle_R");
	GetWorld()->SpawnActor<ACPP_BlasterBeam>(R_BlasterBeam, R_RifleLocation, R_RifleRotation);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BlastEffectAsset, R_RifleLocation);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), BlasterSound, R_RifleLocation);
}

void ACPP_Drone::DelayTimer()
{
	GetWorld()->GetTimerManager().SetTimer(BlastDelay, this, &ACPP_Drone::FireBlasters, DelayBlast, true);
}




