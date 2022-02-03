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
#include "MazeBuild.generated.h"

static const int MIN_DIMENSION_SIZE = 1; // minimum size of any dimension
static const int MAX_DIMENSION_SIZE = 8; // maximum size of any dimension
static constexpr double DEFAULT_SIZE = 100.0; // size of UE4 grid
static constexpr double TEXT_SCALE = 2.0;
static const FColor NO_COLOR = FColor::Black;
static const FColor U_COLOR = FColor::Red;
static const FColor V_COLOR = FColor::Green;
static const FColor W_COLOR = FColor::Blue;
static const FColor DIMENSION_TEXT_COLOR[12] = { U_COLOR, U_COLOR, V_COLOR, V_COLOR, W_COLOR, W_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR, NO_COLOR };
static const FText DIMENSION_PLUS = FText::FromString(TEXT("+"));
static const FText DIMENSION_MINUS = FText::FromString(TEXT("-"));
static const FText DIMENSION_TEXT[12] = { DIMENSION_PLUS, DIMENSION_MINUS, DIMENSION_PLUS, DIMENSION_MINUS, DIMENSION_PLUS, DIMENSION_MINUS, DIMENSION_PLUS, DIMENSION_MINUS, DIMENSION_PLUS, DIMENSION_MINUS, DIMENSION_PLUS, DIMENSION_MINUS };
static const char DIMENSION_INCDEC_CHAR[12] = { '+', '-', '+', '-', '+', '-', '+', '-', '+', '-', '+', '-' };
static const char DIMENSION_CHAR[12] = { 'U', 'U', 'V', 'V', 'W', 'W', 'Z', 'Z', 'Y', 'Y', 'X', 'X' };

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
static const int MIN_VIRTUAL_DIRECTION = U_PLUS;
static const int MAX_VIRTUAL_DIRECTION = W_MINUS;
static const int MIN_REAL_DIRECTION = UP_;
static const int MAX_REAL_DIRECTION = WEST;
static const FRotator WALL_ROTATOR[DIRECTION_COUNT] = { NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, X_ROTATOR, X_ROTATOR, Y_ROTATOR, Y_ROTATOR, NO_ROTATOR, NO_ROTATOR };

static const std::set<Direction> wallDirs = { UP_, SOUTH, EAST };


UCLASS()
class MULTIDIMENSIONMAZE_API AMazeBuild : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		class USoundWave* WinSoundWave;
	UFUNCTION(BlueprintImplementableEvent, Category="Output")
		void setWin();
	// Sets default values for this actor's properties
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
	void NewMaze(int u_size, int v_size, int w_size, int depth, int height, int width);
	void startMaze();
	Position MoveDimension(Position pos, Direction dir);
	void ChangeDirection(Direction dir);
	Position locationToPosition(int u, int v, int w, FVector location);
	FVector positionToLocation(Position pos);
	Maze GetMaze();
	void changeWall();
	void playWin();

private:
	void OuterWall(Position pos);
	void InnerWall(Position pos);
	void Termination(Position pos);
	void RotateDimensionLetters(USceneComponent* letters, Direction dir);
	UInstancedStaticMeshComponent* DimensionLetter(Position pos, Direction dir);
	void DimensionText(Position pos);
	void ProcessCell(Position pos);
	void clearBuild();
	void DisplayMaze();
	void SetDimension(Position pos);
	FVector positionToLocation(Position pos, FVector delta);

	UInstancedStaticMeshComponent* createWall(Direction dir, Position pos);
	UInstancedStaticMeshComponent* createTermination(Direction dir, bool isStart);
	static const FVector DELTA_WALL[DIRECTION_COUNT];

	int selectU = 0;
	int selectV = 0;
	int selectW = 0;

	UStaticMesh* WallMeshRef;
	UStaticMesh* PlusMeshRef;
	UStaticMesh* MinusMeshRef;
	UStaticMesh* TerminationMeshRef;
	UMaterial* StartMaterial;
	UMaterial* EndMaterial;
	UMaterial* Wall1Material;
	UMaterial* Wall2Material;
	UMaterial* Wall3Material;
	UMaterial* Wall4Material;
	UMaterial* Wall5Material;
	UMaterial* WallMaterial;
	UMaterial* UDimensionMaterial;
	UMaterial* VDimensionMaterial;
	UMaterial* WDimensionMaterial;
	BacktrackGenerator backtrackGenerator;
private:
 	std::vector<UMaterial*> DIMENSION_MATERIAL;
	std::vector<UInstancedStaticMeshComponent*> DIM_LETTERS;

	FRotator DIM_LETTER_ROTATORS[12] = { NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, NO_ROTATOR, X_ROTATOR, X_ROTATOR, Y_ROTATOR, Y_ROTATOR, NO_ROTATOR, NO_ROTATOR };
	Maze _maze;
	UInstancedStaticMeshComponent* startTermination;
	UInstancedStaticMeshComponent* endTermination;
	std::vector<std::vector<std::vector<std::vector<UInstancedStaticMeshComponent*>>>> walls; // z, y, x, dir
	std::vector<std::vector<std::vector<USceneComponent*>>> dimensionLetters; // z, y, x -> for rotating letter sets
	std::vector<std::vector<std::vector<std::vector<UInstancedStaticMeshComponent*>>>> dimensionLetter; // z, y, x, dir -> for showing/hiding letters
};
