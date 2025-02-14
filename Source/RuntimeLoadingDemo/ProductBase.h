// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonParserComponent.h"
#include "GameFramework/Actor.h"
#include "ProductBase.generated.h"

UCLASS()
class RUNTIMELOADINGDEMO_API AProductBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProductBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FProductData ProductData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ProductID;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
};
