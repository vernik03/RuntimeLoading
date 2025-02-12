

#include "MeshLoader.h"


AMeshLoader::AMeshLoader()
{
	PrimaryActorTick.bCanEverTick = false;

	JsonParserComponent = CreateDefaultSubobject<UJsonParserComponent>(TEXT("JsonParserComponent"));

}

void AMeshLoader::BeginPlay()
{
	Super::BeginPlay();

	if (JsonParserComponent)
	{
		JsonParserComponent->OnJsonDataParsed.AddDynamic(this, &AMeshLoader::OnJsonDataParsed);
	}

	JsonParserComponent->DownloadJson(JsonUrl);
	
}

void AMeshLoader::OnJsonDataParsed()
{
	UE_LOG(LogTemp, Log, TEXT("Json Data Parsed"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Json Data Parsed"));
	for (auto ProductData : JsonParserComponent->GetProductDataCollection())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, ProductData.Value.Name);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, ProductData.Value.ImagePath);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, ProductData.Value.ModelPath);
	}

}


