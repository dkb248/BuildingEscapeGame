// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "GrabberPawn.h"

#define OUT

// Sets default values for this component's properties
UGrabberPawn::UGrabberPawn()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabberPawn::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty"));
	
}


// Called every frame
void UGrabberPawn::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Get the player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointLocation,
		OUT	PlayerViewPointRotation
	);

	// TODO log out to test

	//FString PlayerViewLoc = GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		//OUT	PlayerViewPointLocation).ToString();

	//FString ObjectPos = GetOwner()->GetTransform().GetLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), 
		*PlayerViewPointLocation.ToString(),
		*PlayerViewPointRotation.ToString()
	);

	// Raycast out to reach distance

	// See what we hit
}

