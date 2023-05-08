// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma de Barcelona (UAB). This work is licensed under the terms of the MIT license. For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProceduralMeshComponent.h"
#include "Math/Vector2D.h"

#include <compiler/disable-ue4-macros.h>
#include <boost/optional.hpp>
#include <carla/road/Map.h>
#include <compiler/enable-ue4-macros.h>

#include "OpenDriveToMap.generated.h"


class UProceduralMeshComponent;
class UMeshComponent;
class UCustomFileDownloader;
class UMaterialInstance;
/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
class CARLATOOLS_API UOpenDriveToMap : public UObject
{
  GENERATED_BODY()

public:

  UFUNCTION()
  void ConvertOSMInOpenDrive();

  UFUNCTION( BlueprintCallable )
  void CreateMap();

  UFUNCTION(BlueprintCallable)
  void CreateTerrain(const int SectionPerSize, const int MeshGridSize, const float MeshGridSectionSize,
     const class UTexture2D* HeightmapTexture);

  UFUNCTION(BlueprintCallable)
  void CreateTerrainMesh(const int MeshIndex, const FVector2D Offset, const int GridSize, const float GridSectionSize,
     const class UTexture2D* HeightmapTexture, class UTextureRenderTarget2D* RoadMask);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="File")
  FString FilePath;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="File")
  FString MapName;

  UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Settings" )
  FString Url;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
  FVector2D OriginGeoCoordinates;

  UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Settings" )
  float DistanceBetweenTrees = 50.0f;
  UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Settings" )
  float DistanceFromRoadEdge = 3.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UMaterialInstance* DefaultRoadMaterial;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UMaterialInstance* DefaultLaneMarksMaterial;

protected:

  UFUNCTION( BlueprintCallable )
  void SaveMap();

  UFUNCTION( BlueprintImplementableEvent )
  void GenerationFinished();
private:

  UFUNCTION()
  void OpenFileDialog();

  UFUNCTION()
  void LoadMap();

  void GenerateAll(const boost::optional<carla::road::Map>& CarlaMap);
  void GenerateRoadMesh(const boost::optional<carla::road::Map>& CarlaMap);
  void GenerateSpawnPoints(const boost::optional<carla::road::Map>& CarlaMap);
  void GenerateTreePositions(const boost::optional<carla::road::Map>& CarlaMap);
  void GenerateLaneMarks(const boost::optional<carla::road::Map>& CarlaMap);

  float GetHeight(float PosX, float PosY);
  carla::rpc::OpendriveGenerationParameters opg_parameters;

  UStaticMesh* CreateStaticMeshAsset(UProceduralMeshComponent* ProcMeshComp, int32 MeshIndex, FString FolderName);
  TArray<UStaticMesh*> CreateStaticMeshAssets();

  UPROPERTY()
  UCustomFileDownloader* FileDownloader;
  UPROPERTY()
  TArray<AActor*> ActorMeshList;
  UPROPERTY()
  TArray<AActor*> LaneMarkerActorList;
  UPROPERTY()
  TArray<UStaticMesh*> MeshesToSpawn;
  UPROPERTY()
  TArray<FString> RoadType;
  UPROPERTY()
  TArray<UProceduralMeshComponent*> RoadMesh;
};
