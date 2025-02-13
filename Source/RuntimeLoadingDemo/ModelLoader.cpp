

#include "ModelLoader.h"


AModelLoader::AModelLoader()
{
	PrimaryActorTick.bCanEverTick = false;

	JsonParserComponent = CreateDefaultSubobject<UJsonParserComponent>(TEXT("JsonParserComponent"));

	ModelFilesManagerComponent = CreateDefaultSubobject<UModelFilesManagerComponent>(TEXT("ModelFilesManagerComponent"));

}

void AModelLoader::BeginPlay()
{
	Super::BeginPlay();

	if (JsonParserComponent)
	{
		JsonParserComponent->OnJsonDataParsed.AddDynamic(this, &AModelLoader::OnJsonDataParsed);
	}

	if (ModelFilesManagerComponent)
	{
		ModelFilesManagerComponent->OnEnvironmentModelDownloaded.AddDynamic(this, &AModelLoader::LoadEnvironment);
		ModelFilesManagerComponent->OnProductFullyDownloaded.AddDynamic(this, &AModelLoader::ProductDownloaded);
	}

	JsonParserComponent->DownloadJson(JsonUrl);
	
}

void AModelLoader::OnJsonDataParsed()
{
	UE_LOG(LogTemp, Log, TEXT("Json Data Parsed"));

	FString& EnvPath = JsonParserComponent->GetSceneDataPath();

	if (ModelFilesManagerComponent)
	{
		ModelFilesManagerComponent->DownloadEnvironmentUSDZ(EnvPath, ModelsPath);
	}

	TMap<FString, FProductData>& Products = JsonParserComponent->GetProductDataCollection();
	for (auto& Pair : Products)
	{
		FString ProductID = Pair.Key;
		FProductData& ProductData = Pair.Value;

		if (ModelFilesManagerComponent)
		{
			ModelFilesManagerComponent->DownloadProductAssets(ProductID, ProductData, ModelsPath);
		}
	}

}