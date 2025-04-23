#include "CPP_SphereSpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "CPP_SmallSphere.h"

// Sets default values
ACPP_SphereSpawner::ACPP_SphereSpawner()
{
	// Enable Tick() to run every frame (optional: can be disabled if not needed)
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACPP_SphereSpawner::BeginPlay()
{
	Super::BeginPlay();

	// Start spawning spheres on a repeating timer
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ACPP_SphereSpawner::SpawnSphere, SpawnTime, true);
}

// Called every frame
void ACPP_SphereSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Spawns a small sphere at a random point within a radius around the spawner
void ACPP_SphereSpawner::SpawnSphere()
{
	// Generate a random direction from a random rotation
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::RandomRotator());

	// Choose a random distance within the defined spawn radius
	float SpawnRange = UKismetMathLibrary::RandomFloatInRange(0, SpawnRadius);

	// Calculate the spawn location based on spawner's position + random offset
	FVector SpawnPoint = GetActorLocation() + ForwardVector * SpawnRange;

	// Spawn the sphere at the location with a random rotation
	GetWorld()->SpawnActor<ACPP_SmallSphere>(SmallSphere, SpawnPoint, UKismetMathLibrary::RandomRotator());
}
