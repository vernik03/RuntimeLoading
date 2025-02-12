

#include "JsonParserComponent.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"


UJsonParserComponent::UJsonParserComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UJsonParserComponent::DownloadJson(FString Url)
{
    FHttpModule* Http = &FHttpModule::Get();
    if (!Http) return;

    const TSharedRef<IHttpRequest> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &UJsonParserComponent::OnJsonDownloadCompleted);
    Request->SetURL(Url);
    Request->SetVerb(TEXT("GET"));
    Request->ProcessRequest();
}


void UJsonParserComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UJsonParserComponent::OnJsonDownloadCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to download JSON or invalid response"));
        return;
    }

    FString ResponseString = Response->GetContentAsString();
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        SceneDataPath = JsonObject->GetStringField(TEXT("environment_model"));
        UE_LOG(LogTemp, Log, TEXT("Environment Model: %s"), *SceneDataPath);

        const TSharedPtr<FJsonObject> ProductsObject = JsonObject->GetObjectField(TEXT("products"));
        if (ProductsObject.IsValid())
        {
            for (const auto& Entry : ProductsObject->Values)
            {
                const FString& ProductID = Entry.Key;
                TSharedPtr<FJsonObject> JsonProductData = Entry.Value->AsObject();

                if (JsonProductData.IsValid())
                {
                    FProductData ProductData;
                    ProductData.Name = JsonProductData->GetStringField(TEXT("name"));
                    ProductData.ImagePath = JsonProductData->GetStringField(TEXT("image_path"));
                    ProductData.ModelPath = JsonProductData->GetStringField(TEXT("model_path"));

                    ProductDataCollection.Add(ProductID, ProductData);

                    UE_LOG(LogTemp, Log, TEXT("ProductID: %s - Name: %s, Image: %s, Model: %s"),
                        *ProductID, *ProductData.Name, *ProductData.ImagePath, *ProductData.ModelPath);

                }
            }

            OnJsonDataParsed.Broadcast();
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON"));
    }
}