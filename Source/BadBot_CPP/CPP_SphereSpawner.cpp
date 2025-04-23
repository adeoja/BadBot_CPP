#include "CPP_SphereSpawner.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ACPP_SphereSpawner::ACPP_SphereSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_SphereSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ACPP_SphereSpawner::SpawnSphere, SpawnTime, true);
}

// Called every frame
void ACPP_SphereSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ACPP_SphereSpawner::SpawnSphere()
{
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::RandomRotator());
	float SpawnRange = UKismetMathLibrary::RandomFloatInRange(0, SpawnRadius);
	FVector SpawnPoint = ForwardVector * SpawnRange + GetActorLocation();
	GetWorld()->SpawnActor<ACPP_SmallSphere>(SmallSphere, SpawnPoint, UKismetMathLibrary::RandomRotator());
}

