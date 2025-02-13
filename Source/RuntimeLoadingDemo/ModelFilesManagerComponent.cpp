

#include "ModelFilesManagerComponent.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"

UModelFilesManagerComponent::UModelFilesManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UModelFilesManagerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UModelFilesManagerComponent::DownloadEnvironmentUSDZ(FString& InOutSceneDataPath, const FString& ModelsPath)
{
    if (InOutSceneDataPath.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("EnvironmentUrl is empty!"));
        return;
    }

    FString FileName = FPaths::GetCleanFilename(InOutSceneDataPath);
    if (FileName.IsEmpty()) FileName = TEXT("Environment.usdz");
    const FString EnvironmentDir = ModelsPath / TEXT("Environment") / FileName;

    TFunction<void(const FString&)> OnDone = [this, &InOutSceneDataPath](const FString& LocalPath)
        {
            InOutSceneDataPath = LocalPath;
            OnEnvironmentModelDownloaded.Broadcast();
        };

    DownloadFile(InOutSceneDataPath, EnvironmentDir, OnDone);
}

void UModelFilesManagerComponent::DownloadProductAssets(const FString& ProductID, FProductData& InOutProductData, const FString& ModelsPath)
{
    if (!InOutProductData.ImagePath.IsEmpty())
    {
        FString ImageFileName = ProductID + TEXT("_") + FPaths::GetCleanFilename(InOutProductData.ImagePath);
        const FString ImageDir = ModelsPath / TEXT("Products") / ProductID / ImageFileName;

        TFunction<void(const FString&)> OnImageDone = [this, &InOutProductData, ProductID](const FString& LocalPath)
            {
                InOutProductData.ImagePath = LocalPath;
                UE_LOG(LogTemp, Log, TEXT("Downloaded product image for %s -> %s"), *ProductID, *LocalPath);
            };

        DownloadFile(InOutProductData.ImagePath, ImageDir, OnImageDone);
    }

    if (!InOutProductData.ModelPath.IsEmpty())
    {
        FString ModelFileName = ProductID + TEXT("_") + FPaths::GetCleanFilename(InOutProductData.ModelPath);

        const FString ModelDir = ModelsPath / TEXT("Products") / ProductID / ModelFileName;

        TFunction<void(const FString&)> OnModelDone = [this, &InOutProductData, ProductID](const FString& LocalPath)
            {
                InOutProductData.ModelPath = LocalPath;
                UE_LOG(LogTemp, Log, TEXT("Downloaded product model for %s -> %s"), *ProductID, *LocalPath);

                InOutProductData.bIsLoaded = true;
                OnProductFullyDownloaded.Broadcast(ProductID);
            };

        DownloadFile(InOutProductData.ModelPath, ModelDir, OnModelDone);
    }
}

void UModelFilesManagerComponent::DownloadFile(const FString& RemoteUrl, const FString& LocalFileName,
    TFunction<void(const FString&)> OnDone)
{
    FHttpModule* Http = &FHttpModule::Get();
    if (!Http) return;

    TSharedRef<IHttpRequest> Request = Http->CreateRequest();
    Request->SetURL(RemoteUrl);
    Request->SetVerb(TEXT("GET"));

    Request->OnProcessRequestComplete().BindUObject(
        this,
        &UModelFilesManagerComponent::OnFileDownloadCompleted,
        LocalFileName,
        OnDone
    );

    Request->ProcessRequest();
}

void UModelFilesManagerComponent::OnFileDownloadCompleted(
    FHttpRequestPtr Request,
    FHttpResponsePtr Response,
    bool bWasSuccessful,
    FString LocalFileName,
    TFunction<void(const FString&)> OnDone)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to download file from: %s"), *Request->GetURL());
        return;
    }

    const TArray<uint8>& FileData = Response->GetContent();

    const FString SaveDir = FPaths::ProjectSavedDir();
    const FString FullFilePath = SaveDir / LocalFileName;

    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    PlatformFile.CreateDirectoryTree(*SaveDir);

    if (FFileHelper::SaveArrayToFile(FileData, *FullFilePath))  
    {
        UE_LOG(LogTemp, Log, TEXT("File saved to: %s"), *FullFilePath);
        OnDone(FullFilePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save file to: %s"), *FullFilePath);
    }
}
