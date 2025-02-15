// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProductIcon.generated.h"

/**
 * 
 */
UCLASS()
class RUNTIMELOADINGDEMO_API UProductIcon : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "ImageLoader")
	bool LoadImageFromPath(const FString& ImagePath, class UImage* TargetImage);
};