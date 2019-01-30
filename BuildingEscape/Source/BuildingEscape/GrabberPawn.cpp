// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "GrabberPawn.h"

#define OUT

// Sets default values for this component's properties
UGrabberPawn::UGrabberPawn()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabberPawn::BeginPlay()
{	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Begin play called"))
	FindPhysicsHandleComponent();
	SetupInputComponent(); }

///Look for attached physics handle
void UGrabberPawn::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
		if (PhysicsHandle == nullptr) 
	{ 
		UE_LOG(LogTemp, Error, TEXT("Physics handle of %s not found"), *GetOwner()->GetName());}
}

///Look for attached input component (only appears at runtime)
void UGrabberPawn::SetupInputComponent() {	

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabberPawn::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabberPawn::Release); }
	else {UE_LOG(LogTemp, Error, TEXT("Input component of %s not found"), *GetOwner()->GetName());}
}

// Called every frame
void UGrabberPawn::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {PhysicsHandle->SetTargetLocation(GetReachLineEnd());}
	//move the held object each frame
}

void UGrabberPawn::Grab() {
	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponenttoGrab = HitResult.GetComponent(); // Gets the mesh in our case
	auto ActorHit = HitResult.GetActor();

	/// If something is hit then try and attach a physics handle
	if (ActorHit) {
		PhysicsHandle->GrabComponent
		(ComponenttoGrab, 
		NAME_None, // no bones needed
		ComponenttoGrab->GetOwner()->GetActorLocation(), 
		true // allows rotation 
		);}
};

void UGrabberPawn::Release() {
		PhysicsHandle->ReleaseComponent();
}


const FHitResult UGrabberPawn::GetFirstPhysicsBodyInReach()
{
	
	/// Line-trace aka Raycast out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult, 
		GetReachLineStart(), 
		GetReachLineEnd(), 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
		TraceParameters);

	return HitResult;
}

FVector UGrabberPawn::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}

FVector UGrabberPawn::GetReachLineEnd() 
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}