// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonParserComponent.h"
#include "GameFramework/Actor.h"
#include "MeshLoader.generated.h"

UCLASS()
class RUNTIMELOADINGDEMO_API AMeshLoader : public AActor
{
	GENERATED_BODY()
	
public:	
	AMeshLoader();

	UPROPERTY()
	UJsonParserComponent* JsonParserComponent;

	UFUNCTION()
	void OnJsonDataParsed();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString JsonUrl;

protected:
	virtual void BeginPlay() override;
	
};
