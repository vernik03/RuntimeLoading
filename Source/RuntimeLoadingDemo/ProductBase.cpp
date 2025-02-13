// Fill out your copyright notice in the Description page of Project Settings.


#include "ProductBase.h"


// Sets default values
AProductBase::AProductBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProductBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProductBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

