#include "CPP_SmallSphere.h"
#include "Kismet/KismetMathLibrary.h"
#include "CPP_BadBotGamemode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"


// Sets default values
ACPP_SmallSphere::ACPP_SmallSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;
}

// Called when the game starts or when spawned
void ACPP_SmallSphere::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	DefaultPawn = PlayerController->GetPawn();

	BadBotGameMode = Cast<ACPP_BadBotGamemode>(UGameplayStatics::GetGameMode(this));

	SphereRotation = UKismetMathLibrary::RandomRotator() * .05;

	GetWorld()->GetTimerManager().SetTimer(WinkOutTimer, this, &ACPP_SmallSphere::WinkOut, 4, false);

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACPP_SmallSphere::OnSphereOverlap);
}

// Called every frame
void ACPP_SmallSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	AddActorWorldOffset(GetActorForwardVector() * DeltaTime * SphereSpeed);

	AddActorWorldRotation(SphereRotation * DeltaTime);

	if (bStartScaling)
	{
		FVector CurrentScale = GetActorScale3D();
		FVector TargetScale = FVector(0 , 0, 0);
		FVector Shrink = UKismetMathLibrary::VInterpTo(CurrentScale, TargetScale, DeltaTime, 10);
		SetActorScale3D(Shrink);
		
		if (FVector::Dist(Shrink, TargetScale) <= 0.01f)
		{
			Destroy();
		}
	}
}

void ACPP_SmallSphere::WinkOut()
{
	bStartScaling = true;
}

void ACPP_SmallSphere::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
					bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == DefaultPawn && BadBotGameMode && !IsWinking)
	{
		IsWinking = true;
		BadBotGameMode->IncrementScore();
		ACPP_SmallSphere::WinkOut();
	}
}



