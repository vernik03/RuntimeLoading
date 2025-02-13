// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseAnchor.h"


// Sets default values
AMouseAnchor::AMouseAnchor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMouseAnchor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMouseAnchor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

