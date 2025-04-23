#include "CPP_BlasterBeam.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ACPP_BlasterBeam::ACPP_BlasterBeam()
{
    // Enable Tick() to run every frame (can be turned off for optimization if unused)
    PrimaryActorTick.bCanEverTick = true;

    // Create and attach the static mesh component for the beam
    BlasterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlasterMesh"));
    RootComponent = BlasterMesh;
}

// Called when the game starts or when spawned
void ACPP_BlasterBeam::BeginPlay()
{
    Super::BeginPlay();

    // Get reference to the player's pawn
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    DefaultPawn = PlayerController ? PlayerController->GetPawn() : nullptr;

    // Bind the OnHit event to the blaster mesh
    BlasterMesh->OnComponentHit.AddDynamic(this, &ACPP_BlasterBeam::OnHit);
}

// Called every frame
void ACPP_BlasterBeam::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Function triggered when the blaster beam hits another actor
void ACPP_BlasterBeam::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
                             UPrimitiveComponent* OtherComp, FVector NormalImpulse, 
                             const FHitResult& Hit)
{
    // If the beam hits the player's pawn, restart the level
    if (OtherActor == DefaultPawn)
    {
        UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
    }

    // Spawn a Niagara visual effect at the hit location with correct orientation
    if (BeamBurst)
    {
        FRotator ImpactRotation = UKismetMathLibrary::MakeRotFromX(Hit.Normal);
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BeamBurst, Hit.Location, ImpactRotation);
    }

    // Destroy the blaster mesh so it visually disappears after impact
    BlasterMesh->DestroyComponent();
}
