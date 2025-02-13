
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "JsonParserComponent.generated.h"

USTRUCT(BlueprintType)
struct FProductData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ImagePath;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ModelPath;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsLoaded = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJsonDataParsed);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUNTIMELOADINGDEMO_API UJsonParserComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UJsonParserComponent();

	UFUNCTION(BlueprintCallable, Category = "JSON")
	void DownloadJson(FString Url);

	UPROPERTY(BlueprintAssignable, Category = "JSON")
	FOnJsonDataParsed OnJsonDataParsed;

	UFUNCTION(BlueprintCallable, Category = "Data")
	TMap<FString, FProductData>& GetProductDataCollection() { return ProductDataCollection; }

	UFUNCTION(BlueprintCallable, Category = "Data")
	FString& GetSceneDataPath() { return SceneDataPath; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TMap<FString, FProductData> ProductDataCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString SceneDataPath;
		
private:
	void OnJsonDownloadCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};
