#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/UObjectGlobals.h"
#include "maze_generator_core/dirs.h"
#include <vector>
#include <string>
#include <set>
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/PointLightComponent.h"
#include "maze_generators/BacktrackGenerator.h"
#include "maze_generator_core/Maze.h"
#include "Math/Color.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "MazeBuild.generated.h"

static const int MIN_DIMENSION_SIZE = 1; // minimum size of any dimension
static const int MAX_DIMENSION_SIZE = 8; // maximum size of any dimension
static constexpr double DEFAULT_SIZE = 100.0; // size of UE4 grid
static constexpr double TEXT_SCALE = 2.0;
static const FColor NO_COLOR = FColor::Black;
static const FColor TERMINATION_COLOR = FColor::Orange;

static const FText DIMENSION_PLUS = FText::FromString(TEXT("+"));
static const FText DIMENSION_MINUS = FText::FromString(TEXT("-"));
static const FText DIMENSION_TEXT[12] = { DIMENSION_PLUS, DIMENSION_MINUS, DIMENSION_PLUS, DIMENSION_MINUS, DIMENSION_PLUS, DIMENSION_MINUS, DIMENSION_PLUS, DIMENSION_MINUS, DIMENSION_PLUS, DIMENSION_MINUS, DIMENSION_PLUS, DIMENSION_MINUS };
static const char DIMENSION_INCDEC_CHAR[12] = { '+', '-', '+', '-', '+', '-', '+', '-', '+', '-', '+', '-' };

static const double DIMENSION_TEXT_X = +0.5;
static const double DIMENSION_TEXT_Y = +0.5;//+0.75;//+0.9;
static const double DIMENSION_TEXT_Z = +0.5;//-0.35;

static const FVector DIMENSION_TEXT_DELTA(0.0, 0.15, -0.3);

static const double DIMENSION_TEXT_HORZ[12] = { -0.25, -0.25, 0, 0, 0.25, 0.25, 0, 0, 0, 0, 0, 0 };
static const double DIMENSION_TEXT_VERT[12] = { 0.1, -0.1, 0.1, -0.1, 0.1, -0.1, 0, 0, 0, 0, 0, 0 };

static const FRotator NO_ROTATOR = FRotator(0.0, 0.0, 0.0);
static const FRotator X_ROTATOR = FRotator(90.0, 0.0, 0.0);
static const FRotator Y_ROTATOR = FRotator(0.0, 90.0, 0.0);
static const FRotator Z_ROTATOR = FRotator(0.0, 0.0, 90.0);
static const FRotator YY_ROTATOR = FRotator(0.0, 180.0, 0.0);
static const FRotator YYY_ROTATOR = FRotator(0.0, 270.0, 0.0);
static const FRotator X_Y_ROTATOR = FRotator(90.0, 90.0, 0.0);
static const FRotator X_YY_ROTATOR = FRotator(90.0, 180.0, 0.0);
static const FRotator X_YYY_ROTATOR = FRotator(90.0, 270.0, 0.0);
static const FRotator XXX_ROTATOR = FRotator(270.0, 0.0, 0.0);
static const FRotator XXX_Y_ROTATOR = FRotator(270.0, 90.0, 0.0);
static const FRotator XXX_YY_ROTATOR = FRotator(270.0, 180.0, 0.0);
static const FRotator XXX_YYY_ROTATOR = FRotator(270.0, 270.0, 0.0);
static const int MIN_VIRTUAL_DIRECTION = A_PLUS;
static const int MAX_VIRTUAL_DIRECTION = C_MINUS;
static const int MIN_REAL_DIRECTION = UP_;
static const int MAX_REAL_DIRECTION = WEST;
static const int TEXTURE_COUNT = 5;

static FText START_FTEXT = FText::FromString(TEXT("start"));
static FText END_FTEXT = FText::FromString(TEXT("end"));

static const FRotator WALL_ROTATOR[DIRECTION_COUNT] = { NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, X_ROTATOR, X_ROTATOR, Y_ROTATOR, Y_ROTATOR, NO_ROTATOR, NO_ROTATOR };

static const std::set<Direction> wallDirs = { UP_, SOUTH, EAST };

UCLASS()
class MULTIDIMENSIONMAZE_API AMazeBuild : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		class UParticleSystem* FireworksParticleSystem;
	UFUNCTION(BlueprintImplementableEvent, Category = "Output")
		void playFireworksSound();
	UFUNCTION(BlueprintImplementableEvent, Category = "Output")
		void startShowWin();
	UFUNCTION(BlueprintImplementableEvent, Category = "Output")
		void endShowWin();
	UFUNCTION(BlueprintImplementableEvent, Category = "Output")
		void displayRandSeed(int randSeed);
	UFUNCTION(BlueprintImplementableEvent, Category = "Output")
		void displayDimensionTransition(float displayPercent);
	UFUNCTION(BlueprintImplementableEvent, Category = "Output")
		void finishAnimation();

	static constexpr double CUBE_ACTUAL_SIZE = 80.0;
	static constexpr double CUBE_SIZE = CUBE_ACTUAL_SIZE / DEFAULT_SIZE;
	static constexpr double WALL_RATIO = 1.0 / 100.0;
	static constexpr double WALL_ACTUAL_THICKNESS = CUBE_ACTUAL_SIZE * WALL_RATIO;
	static constexpr double WALL_THICKNESS = WALL_ACTUAL_THICKNESS / DEFAULT_SIZE;
	static constexpr double WALL_SIZE = CUBE_SIZE;
	static constexpr double TERMINATION_DIAMETER = CUBE_SIZE / 8.0;
	static constexpr double DIM_LETTER_SHORT = CUBE_SIZE / 24;
	static constexpr double DIM_LETTER_LONG = 3 * DIM_LETTER_SHORT;

	AMazeBuild();
	void BeginPlay();
	void NewMaze(int a_size, int b_size, int c_size, int depth, int height, int width);
	void startMaze(FRotator playerRotator);
	Position MoveDimension(Position pos, Direction dir);
	void ChangeDirection(Direction dir, FRotator playerRotator);
	Position locationToPosition(int u, int v, int w, FVector location);
	FVector positionToLocation(Position pos, FVector delta);
	FVector positionToLocation(Position pos);
	Maze GetMaze();
	void changeWall();
	void playWin();

private:
	void OuterWall(Position pos);
	void InnerWall(Position pos);
	void RotateDimensionLetters(USceneComponent* letters, Direction dir, FRotator playerRotator);
	UInstancedStaticMeshComponent* DimensionLetter(Position pos, Direction dir);
	void DimensionText(Position pos);
	void ProcessCell(Position pos);
	void clearBuild();
	void DisplayMaze();
	void SetDimension(Position pos);
	void TriggerFireworks();

	UInstancedStaticMeshComponent* createWall(Direction dir, Position pos);
	UInstancedStaticMeshComponent* createTermination(Direction dir, bool isStart);
	static constexpr float FIREWORKS_DELTA_TIME = 0.5f;
	static constexpr float FIREWORKS_DELTA_DELTA_END_LOCATION = 10.0f;
	static const FVector DELTA_WALL[DIRECTION_COUNT];
	static const FVector FIREWORKS_DELTA_END_LOCATION[9];
	static const int WALL_TEXTURE_COUNT = 1 + WEST - UP_;

	int selectA = 0;
	int selectB = 0;
	int selectC = 0;

	UStaticMesh* WallMeshRef;
	UStaticMesh* PlusMeshRef;
	UStaticMesh* MinusMeshRef;
	UStaticMesh* StartMeshRef;
	UStaticMesh* EndMeshRef;
	UMaterial* StartMaterial;
	UMaterial* EndMaterial;

	UMaterialInstanceDynamic* DynamicMaterial;
	UMaterial* WallTestMaterial;
	UMaterialInstanceDynamic* WallTest2Material;
	UMaterial* WallParam;
	int WallTextureIndex = 0;
	UMaterialInstanceDynamic* WallMaterials[WALL_TEXTURE_COUNT];
	UTexture2D* TextureColor[TEXTURE_COUNT];
	UTexture2D* TextureNormal[TEXTURE_COUNT];
	UMaterial* ADimensionMaterial;
	UMaterial* BDimensionMaterial;
	UMaterial* CDimensionMaterial;
	BacktrackGenerator backtrackGenerator;
private:
	FTimerHandle TriggerFireworkTimerHandle;
	int fireworksCount;
 	std::vector<UMaterial*> DIMENSION_MATERIAL;
	std::vector<UInstancedStaticMeshComponent*> DIM_LETTERS;

	FRotator DIM_LETTER_ROTATORS[12] = { NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, X_ROTATOR, X_ROTATOR, Y_ROTATOR, Y_ROTATOR, NO_ROTATOR, NO_ROTATOR };
	FRotator TERMINATION_ROTATORS[12] = { NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, X_ROTATOR, X_ROTATOR, YYY_ROTATOR, Y_ROTATOR, YY_ROTATOR, NO_ROTATOR };
	FRotator TERMINATION_DOWN_ROTATORS[4] = { X_YY_ROTATOR, X_YYY_ROTATOR, X_ROTATOR, X_Y_ROTATOR };
	FRotator TERMINATION_UP_ROTATORS[4] = { XXX_YY_ROTATOR, XXX_YYY_ROTATOR, XXX_ROTATOR, XXX_Y_ROTATOR };
	Maze _maze;
	ATextRenderActor* startText = NULL;
	ATextRenderActor* endText = NULL;
	std::vector<std::vector<std::vector<std::vector<UInstancedStaticMeshComponent*>>>> walls; // z, y, x, dir
	std::vector<std::vector<std::vector<USceneComponent*>>> dimensionLetters; // z, y, x -> for rotating letter sets
	std::vector<std::vector<std::vector<std::vector<UInstancedStaticMeshComponent*>>>> dimensionLetter; // z, y, x, dir -> for showing/hiding letters
};
