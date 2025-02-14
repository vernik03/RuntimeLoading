

#include "ProductBase.h"


AProductBase::AProductBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
}

void AProductBase::BeginPlay()
{
	Super::BeginPlay();	
}


void AProductBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

