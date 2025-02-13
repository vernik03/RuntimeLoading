

#include "ModelLoader.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/BodySetup.h"


AModelLoader::AModelLoader()
{
	PrimaryActorTick.bCanEverTick = false;

	JsonParserComponent = CreateDefaultSubobject<UJsonParserComponent>(TEXT("JsonParserComponent"));

	ModelFilesManagerComponent = CreateDefaultSubobject<UModelFilesManagerComponent>(TEXT("ModelFilesManagerComponent"));

}

void AModelLoader::EnableActorsCollision(const TArray<AActor*>& ChildActors)
{
    for (AActor* Actor : ChildActors)
    {
        if (!Actor)
        {
            continue;
        }

		Actor->SetActorEnableCollision(true);

        TArray<UStaticMeshComponent*> MeshComponents;
        Actor->GetComponents<UStaticMeshComponent>(MeshComponents);

		EnableComponentsCollision(MeshComponents);
    }
}

void AModelLoader::EnableComponentsCollision(const TArray<UStaticMeshComponent*>& MeshComponents)
{
    for (UStaticMeshComponent* MeshComponent : MeshComponents)
    {
        if (!MeshComponent)
        {
            continue;
        }

        MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

        UStaticMesh* StaticMesh = MeshComponent->GetStaticMesh();
        if (StaticMesh)
        {
            UBodySetup* BodySetup = StaticMesh->GetBodySetup();
            if (BodySetup)
            {
				BodySetup->bNeverNeedsCookedCollisionData = false;
                BodySetup->CollisionTraceFlag = ECollisionTraceFlag::CTF_UseComplexAsSimple;
                BodySetup->CreatePhysicsMeshes();
            	MeshComponent->RecreatePhysicsState();				
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Collision Enabled"));
            }			
        }
    }
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