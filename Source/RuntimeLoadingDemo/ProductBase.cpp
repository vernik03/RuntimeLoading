#include "ProductBase.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "KismetProceduralMeshLibrary.h"
#include "Materials/MaterialInterface.h"
#include "CompGeom/ConvexHull3.h"

AProductBase::AProductBase()
{
    PrimaryActorTick.bCanEverTick = true;

    CombinedPMC = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CombinedProceduralMesh"));
    SetRootComponent(CombinedPMC);

    // Basic collision setup
    CombinedPMC->bUseComplexAsSimpleCollision = false;
    CombinedPMC->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CombinedPMC->SetCollisionObjectType(ECC_WorldDynamic);
    CombinedPMC->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

void AProductBase::BeginPlay()
{
    Super::BeginPlay();
}

void AProductBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AProductBase::CombineMeshes(const TArray<UStaticMeshComponent*>& MeshComponents)
{
    CombinedPMC->ClearAllMeshSections();
    CombinedPMC->ClearCollisionConvexMeshes();

    FVector MinVec(FLT_MAX, FLT_MAX, FLT_MAX);
    FVector MaxVec(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    TArray<FVector> AllCollisionVerts;
    AllCollisionVerts.Reserve(50000);

    for (UStaticMeshComponent* SMComp : MeshComponents)
    {
        if (!SMComp) continue;
        UStaticMesh* SM = SMComp->GetStaticMesh();
        if (!SM) continue;

        const int32 LODIndex = 0;
        const int32 NumSections = SM->GetNumSections(LODIndex);
        for (int32 s = 0; s < NumSections; s++)
        {
            TArray<FVector> Vertices;
            TArray<int32> Triangles;
            TArray<FVector> Normals;
            TArray<FVector2D> UVs;
            TArray<FProcMeshTangent> Tangents;

            UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(
                SM,
                LODIndex,
                s,
                Vertices,
                Triangles,
                Normals,
                UVs,
                Tangents
            );

            for (const FVector& V : Vertices)
            {
                MinVec.X = FMath::Min(MinVec.X, V.X);
                MinVec.Y = FMath::Min(MinVec.Y, V.Y);
                MinVec.Z = FMath::Min(MinVec.Z, V.Z);

                MaxVec.X = FMath::Max(MaxVec.X, V.X);
                MaxVec.Y = FMath::Max(MaxVec.Y, V.Y);
                MaxVec.Z = FMath::Max(MaxVec.Z, V.Z);
            }
        }
    }

    const FVector OffsetVec(0.f, 0.f, -MinVec.Z);

    int32 SectionIndex = 0;
    for (UStaticMeshComponent* SMComp : MeshComponents)
    {
        if (!SMComp) continue;
        UStaticMesh* SM = SMComp->GetStaticMesh();
        if (!SM) continue;

        const int32 LODIndex = 0;
        const int32 NumSections = SM->GetNumSections(LODIndex);
        for (int32 s = 0; s < NumSections; s++)
        {
            TArray<FVector> Vertices;
            TArray<int32> Triangles;
            TArray<FVector> Normals;
            TArray<FVector2D> UVs;
            TArray<FProcMeshTangent> Tangents;

            UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(
                SM,
                LODIndex,
                s,
                Vertices,
                Triangles,
                Normals,
                UVs,
                Tangents
            );

            for (FVector& V : Vertices)
            {
                V += OffsetVec;
            }

            AllCollisionVerts.Append(Vertices);

            CombinedPMC->CreateMeshSection(
                SectionIndex,
                Vertices,
                Triangles,
                Normals,
                UVs,
                TArray<FColor>(),
                Tangents,
                true  
            );

            UMaterialInterface* Mat = SMComp->GetMaterial(s);
            if (!Mat)
            {
                Mat = SM->GetMaterial(s);
            }
            if (Mat)
            {
                CombinedPMC->SetMaterial(SectionIndex, Mat);
            }

            SectionIndex++;
        }
    }

    {
        using namespace UE::Geometry;

        TConvexHull3<double> HullGen; 
        bool bOK = HullGen.Solve(
            TArrayView<const FVector>(AllCollisionVerts)
        );

        if (bOK && HullGen.GetDimension() == 3)
        {
            const TArray<FIndex3i>& HullTriangles = HullGen.GetTriangles();

            TSet<int32> UniqueIndices;
            for (const FIndex3i& Tri : HullTriangles)
            {
                UniqueIndices.Add(Tri.A);
                UniqueIndices.Add(Tri.B);
                UniqueIndices.Add(Tri.C);
            }

            TArray<FVector> HullVerts;
            HullVerts.Reserve(UniqueIndices.Num());
            for (int32 Idx : UniqueIndices)
            {
                HullVerts.Add(AllCollisionVerts[Idx]);
            }

            CombinedPMC->AddCollisionConvexMesh(HullVerts);
        }
    }
}