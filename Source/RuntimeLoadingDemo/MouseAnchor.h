
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MouseAnchor.generated.h"

UCLASS()
class RUNTIMELOADINGDEMO_API AMouseAnchor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMouseAnchor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	
	
};
