// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeLoadingDemoPickUpComponent.h"

URuntimeLoadingDemoPickUpComponent::URuntimeLoadingDemoPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void URuntimeLoadingDemoPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &URuntimeLoadingDemoPickUpComponent::OnSphereBeginOverlap);
}

void URuntimeLoadingDemoPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ARuntimeLoadingDemoCharacter* Character = Cast<ARuntimeLoadingDemoCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
