#include "CPP_Drone.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ACPP_Drone::ACPP_Drone()
{
    // Enable Tick() to run every frame
    PrimaryActorTick.bCanEverTick = true;

    // Create and set the root component for the drone mesh
    DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>("DroneMesh");
    RootComponent = DroneMesh;
}

// Called when the game starts or when spawned
void ACPP_Drone::BeginPlay()
{
    Super::BeginPlay();

    // Get the default pawn (player) reference
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        DefaultPawn = PlayerController->GetPawn();
    }

    // Set a one-time delay timer before the blaster firing loop starts
    GetWorld()->GetTimerManager().SetTimer(StartDelay, this, &ACPP_Drone::DelayTimer, DelayStart, false);
}

// Called every frame
void ACPP_Drone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsValid(DefaultPawn))
    {
        // Get current location and rotation of the drone and the player
        FVector DroneLocation = GetActorLocation();
        FRotator DroneRotation = GetActorRotation();
        FVector PawnLocation = DefaultPawn->GetActorLocation();

        // Smoothly rotate to face the player using interpolated look-at rotation
        FRotator LookAtPawn = UKismetMathLibrary::FindLookAtRotation(DroneLocation, PawnLocation);
        FRotator LookAtPawnInterp = UKismetMathLibrary::RInterpTo(DroneRotation, LookAtPawn, DeltaTime, InterpSpd);
        SetActorRotation(LookAtPawnInterp);

        // Move the drone forward
        FVector MoveLocation = GetActorForwardVector() * DeltaTime * MoveSpeed;
        AddActorWorldOffset(MoveLocation);
    }
}

// Called repeatedly after DelayStart to alternate fire between left and right blasters
void ACPP_Drone::FireBlasters()
{
    // Check if everything is valid before spawning effects
    if (DroneMesh && DroneMesh->DoesSocketExist("Rifle_L") && DroneMesh->DoesSocketExist("Rifle_R")
        && L_BlasterBeam && R_BlasterBeam && BlasterSound && BlastEffectAsset)
    {
        // Alternate between firing from left and right sockets
        if (SwitchRifle)
        {
            Spawn_L_Blaster();
        }
        else
        {
            Spawn_R_Blaster();
        }

        // Flip the toggle for next shot
        SwitchRifle = !SwitchRifle;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Blaster not firing - missing assets or sockets."));
    }
}

// Spawns a blaster and effects from the left rifle socket
void ACPP_Drone::Spawn_L_Blaster()
{
    FVector L_RifleLocation = DroneMesh->GetSocketLocation("Rifle_L");
    FRotator L_RifleRotation = DroneMesh->GetSocketRotation("Rifle_L");

    GetWorld()->SpawnActor<ACPP_BlasterBeam>(L_BlasterBeam, L_RifleLocation, L_RifleRotation);
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BlastEffectAsset, L_RifleLocation);
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), BlasterSound, L_RifleLocation);
}

// Spawns a blaster and effects from the right rifle socket
void ACPP_Drone::Spawn_R_Blaster()
{
    FVector R_RifleLocation = DroneMesh->GetSocketLocation("Rifle_R");
    FRotator R_RifleRotation = DroneMesh->GetSocketRotation("Rifle_R");

    GetWorld()->SpawnActor<ACPP_BlasterBeam>(R_BlasterBeam, R_RifleLocation, R_RifleRotation);
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BlastEffectAsset, R_RifleLocation);
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), BlasterSound, R_RifleLocation);
}

// Called once after DelayStart, then sets up a looping timer to fire blasters
void ACPP_Drone::DelayTimer()
{
    // Begin firing the blasters on a loop with interval DelayBlast
    GetWorld()->GetTimerManager().SetTimer(BlastDelay, this, &ACPP_Drone::FireBlasters, DelayBlast, true);
}
