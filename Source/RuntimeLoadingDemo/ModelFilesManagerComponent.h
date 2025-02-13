

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JsonParserComponent.h" 
#include "ModelFilesManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnvironmentModelDownloaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProductFullyDownloaded, const FString&, ProductID);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RUNTIMELOADINGDEMO_API UModelFilesManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UModelFilesManagerComponent();

    UFUNCTION(BlueprintCallable, Category = "ModelFiles")
    void DownloadEnvironmentUSDZ(FString& InOutSceneDataPath, const FString& ModelsPath);

    UFUNCTION(BlueprintCallable, Category = "ModelFiles")
    void DownloadProductAssets(const FString& ProductID, FProductData& InOutProductData, const FString& ModelsPath);

protected:
    virtual void BeginPlay() override;

private:
    void DownloadFile(const FString& RemoteUrl, const FString& LocalFileName, TFunction<void(const FString&)> OnDone);
    void OnFileDownloadCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful,
        FString LocalFileName, TFunction<void(const FString&)> OnDone);

public:
    UPROPERTY(BlueprintAssignable, Category = "ModelFiles")
    FOnEnvironmentModelDownloaded OnEnvironmentModelDownloaded;

    UPROPERTY(BlueprintAssignable, Category = "ModelFiles")
    FOnProductFullyDownloaded OnProductFullyDownloaded;
};