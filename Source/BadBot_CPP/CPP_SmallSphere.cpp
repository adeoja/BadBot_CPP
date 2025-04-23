#include "CPP_SmallSphere.h"
#include "Kismet/KismetMathLibrary.h"
#include "CPP_BadBotGamemode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ACPP_SmallSphere::ACPP_SmallSphere()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create the sphere collision component and make it the root
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    RootComponent = SphereComp;
}

// Called when the game starts or when spawned
void ACPP_SmallSphere::BeginPlay()
{
    Super::BeginPlay();

    // Get default pawn from player controller
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    DefaultPawn = PlayerController ? PlayerController->GetPawn() : nullptr;

    // Cache the game mode and cast to your custom class
    BadBotGameMode = Cast<ACPP_BadBotGamemode>(UGameplayStatics::GetGameMode(this));

    // Assign a small random rotation for visual spin
    SphereRotation = UKismetMathLibrary::RandomRotator() * 0.05f;

    // Start a one-time timer to trigger WinkOut() after 4 seconds
    GetWorld()->GetTimerManager().SetTimer(WinkOutTimer, this, &ACPP_SmallSphere::WinkOut, 4.0f, false);

    // Bind overlap event to detect player collision
    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACPP_SmallSphere::OnSphereOverlap);
}

// Called every frame
void ACPP_SmallSphere::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Move the sphere forward constantly
    AddActorWorldOffset(GetActorForwardVector() * DeltaTime * SphereSpeed);

    // Apply rotation each frame for a visual spin
    AddActorWorldRotation(SphereRotation * DeltaTime);

    // Handle the shrinking "wink out" animation
    if (bStartScaling)
    {
        FVector CurrentScale = GetActorScale3D();
        FVector TargetScale = FVector(0, 0, 0);

        // Smoothly interpolate scale toward zero
        FVector Shrink = UKismetMathLibrary::VInterpTo(CurrentScale, TargetScale, DeltaTime, 10);
        SetActorScale3D(Shrink);

        // If the sphere is small enough, destroy it
        if (FVector::Dist(Shrink, TargetScale) <= 0.01f)
        {
            Destroy();
        }
    }
}

// Starts the wink-out shrinking process
void ACPP_SmallSphere::WinkOut()
{
    bStartScaling = true;
}

// Handles collision with the player
void ACPP_SmallSphere::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                       bool bFromSweep, const FHitResult& SweepResult)
{
    // If the player touches the sphere, and it hasn't already winked out
    if (OtherActor == DefaultPawn && BadBotGameMode && !IsWinking)
    {
        IsWinking = true;                         // Prevent multiple triggers
        BadBotGameMode->IncrementScore();         // Add to the player's score
        ACPP_SmallSphere::WinkOut();              // Begin the shrinking effect
    }
}
