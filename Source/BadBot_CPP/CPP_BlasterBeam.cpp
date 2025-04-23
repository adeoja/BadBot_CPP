#include "CPP_BlasterBeam.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACPP_BlasterBeam::ACPP_BlasterBeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlasterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlasterMesh"));
	RootComponent = BlasterMesh;
}

// Called when the game starts or when spawned
void ACPP_BlasterBeam::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	DefaultPawn = PlayerController->GetPawn();

	BlasterMesh->OnComponentHit.AddDynamic(this, &ACPP_BlasterBeam::OnHit);
}

// Called every frame
void ACPP_BlasterBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_BlasterBeam::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
							 UPrimitiveComponent* OtherComp, FVector NormalImpulse, 
							 const FHitResult& Hit)
{
	if (OtherActor == DefaultPawn)
	{
		// Restart level on hit
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BeamBurst, Hit.Location, UKismetMathLibrary::MakeRotFromX(Hit.Normal));
	BlasterMesh->DestroyComponent();
}


