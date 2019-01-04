// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "GrabberPawn.h"

#define OUT

// Sets default values for this component's properties
UGrabberPawn::UGrabberPawn()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabberPawn::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Begin play called"))
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

///Look for attached physics handle
void UGrabberPawn::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
		if (PhysicsHandle) {
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physics handle of %s not found"), *GetOwner()->GetName());
	}
}

///Look for attached input component (only appears at runtime)
void UGrabberPawn::SetupInputComponent() {	

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input component found"))
		/// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabberPawn::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabberPawn::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component of %s not found"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabberPawn::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if the physics handle is attached
	//move the held object each frame
}


void UGrabberPawn::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"))
		/// LINE TRACE and see if we reach any actors with physics body collision channel set
		GetFirstPhysicsBodyInReach();

	/// If something is hit then try and attach a physics handle
	//TODO attach a physics handle
};

void UGrabberPawn::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released"))
	//TODO release physics handle
}


const FHitResult UGrabberPawn::GetFirstPhysicsBodyInReach()
{
	/// Get the player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointLocation,
		OUT	PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	/// Set up query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace aka Raycast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), (*ActorHit->GetName()))
	}

	return FHitResult();
}

