#pragma once

#include "CoreMinimal.h"
#include "JsonParserComponent.h"
#include "ProceduralMeshComponent.h" 
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h" 
#include "KismetProceduralMeshLibrary.h"
#include "ProductBase.generated.h"

UCLASS()
class RUNTIMELOADINGDEMO_API AProductBase : public AActor
{
	GENERATED_BODY()

public:
	AProductBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Runtime Mesh")
	UProceduralMeshComponent* CombinedPMC;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FProductData ProductData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ProductID;

	UFUNCTION(BlueprintCallable, Category = "Runtime Mesh")
	void CombineMeshes(const TArray<UStaticMeshComponent*>& MeshComponents);
};
