

#include "ProductBase.h"


AProductBase::AProductBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProductBase::BeginPlay()
{
	Super::BeginPlay();	
}


void AProductBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

