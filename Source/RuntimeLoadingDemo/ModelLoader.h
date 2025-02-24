
#pragma once

#include "CoreMinimal.h"
#include "JsonParserComponent.h"
#include "ModelFilesManagerComponent.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "ModelLoader.generated.h"

UCLASS()
class RUNTIMELOADINGDEMO_API AModelLoader : public AActor
{
	GENERATED_BODY()
	
public:	
	AModelLoader();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString JsonUrl;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ModelsPath = "Models";

	UFUNCTION()
	void OnJsonDataParsed();

	UFUNCTION(BlueprintImplementableEvent, Category = "Load")
	void LoadEnvironment();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Load")
	void LoadProduct(const FString& ProductID);

	UFUNCTION(BlueprintImplementableEvent, Category = "Load")
	void ProductDownloaded(const FString& ProductID);

	UFUNCTION(BlueprintCallable)
	void EnableActorsCollision(const TArray<AActor*>& ChildActors);

	UFUNCTION(BlueprintCallable)
	void EnableComponentsCollision(const TArray<UStaticMeshComponent*>& MeshComponents);


protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
	UJsonParserComponent* JsonParserComponent;

	UPROPERTY(BlueprintReadWrite)
	UModelFilesManagerComponent* ModelFilesManagerComponent;
	
};
