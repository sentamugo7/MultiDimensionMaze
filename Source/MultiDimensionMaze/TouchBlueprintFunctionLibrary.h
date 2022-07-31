#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "maze_generator_core/Maze.h"
#include "MazeBuild.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GeneralProjectSettings.h"
#include <cmath>
#include <math.h>
#include <set>
#include "TouchBlueprintFunctionLibrary.generated.h"

typedef void (*PlayerFunction) (bool isPress);

/**
 * 
 */
UCLASS()
class MULTIDIMENSIONMAZE_API UTouchBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasUp();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasDown();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasLeft();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasRight();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasForward();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasReverse();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasAPlus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasAMinus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasBPlus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasBMinus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasCPlus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasCMinus();

	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool isUp();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool isDown();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void turnPlayerUp(bool isPress);
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void turnPlayerDown(bool isPress);
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void turnPlayerLeft(bool isPress);
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void turnPlayerRight(bool isPress);
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void goPlayerForward(bool isPress);
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void turnPlayerReverse(bool isPress);
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void clickAPlus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void clickAMinus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void clickBPlus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void clickBMinus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void clickCPlus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void clickCMinus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static FString getPlayerPositionADisplay();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static FString getPlayerPositionBDisplay();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static FString getPlayerPositionCDisplay();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static FString getPlayerPositionXDisplay();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static FString getPlayerPositionYDisplay();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static FString getPlayerPositionZDisplay();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static FString getHint();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static FString getOrientation();

	UFUNCTION(BlueprintCallable, Category = "Dim")
		static int getPlayerPositionA();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static int getPlayerPositionB();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static int getPlayerPositionC();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static int getPlayerPositionX();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static int getPlayerPositionY();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static int getPlayerPositionZ();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static FVector getPlayerForward();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static FVector getPlayerPosition();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static float getPlayerRotationPitch();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static float getPlayerRotationYaw();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static float getPlayerRotationRoll();

	UFUNCTION(BlueprintCallable, Category = "Settings")
		static int getASize();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static int getBSize();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static int getCSize();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static int getXSize();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static int getYSize();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static int getZSize();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static int getDimensionSize(int dimensionIndex);
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static void updateSettings(int xSize, int ySize, int zSize, int aSize, int bSize, int cSize);
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static void initTutorialSettings();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static void changeWall();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static void newMap(AMazeBuild* mazeBuild, APlayerController* playerController, ASkyLight* skyLight, float animationDuration);
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static bool isWin();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static void MazeTick(float animationTime);
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static FString getScore();

	UFUNCTION(BlueprintCallable, Category = "State")
		static void setTutorial(bool tutorial);
	UFUNCTION(BlueprintCallable, Category = "State")
		static bool isTutorial();
	UFUNCTION(BlueprintCallable, Category = "State")
		static void setDemo(bool demo);
	UFUNCTION(BlueprintCallable, Category = "State")
		static bool isDemo();
	UFUNCTION(BlueprintCallable, Category = "State")
		static void setTooltip(bool tooltip);
	UFUNCTION(BlueprintCallable, Category = "State")
		static bool isTooltip();
	UFUNCTION(BlueprintCallable, Category = "State")
		static void resetState();

	UFUNCTION(BlueprintPure, Category = "Meta")
		static FString getProjectVersion();

	static void initMaze();
	static void setPlayer(ACharacter* player);
	static void startRotatePlayer(FRotator rotator);
	static void finishRotatePlayer();
	static void startPlayerForward(Position position);
	static void finishPlayerForward();
	static void startPlayerDimension(Direction direction);
	static void switchPlayerDimension();

	static Direction getPlayerForwardDirection();
	static void ShowWin();
	static void playWin();
	static void FinishWin();
	static void MovePlayerTo(Position pos);
	static FVector PLAYER_UP;
	static FVector PLAYER_FORWARD[];
	static ACharacter* _player;

private:
	const static int DEFAULT_DIMENSION_SIZE = 5;
	constexpr static float DEFAULT_PLAYER_SPEED = 0.2;
	constexpr static float DEFAULT_CELL_SIZE = 1.0;
	const static int INIT_X_DIMENSION = 5;
	const static int INIT_Y_DIMENSION = 5;
	const static int INIT_Z_DIMENSION = 1;
	const static int INIT_A_DIMENSION = 1;
	const static int INIT_B_DIMENSION = 1;
	const static int INIT_C_DIMENSION = 1;
	const static int DEMO_X_DIMENSION = 3;
	const static int DEMO_Y_DIMENSION = 3;
	const static int DEMO_Z_DIMENSION = 2;
	const static int DEMO_A_DIMENSION = 1;
	const static int DEMO_B_DIMENSION = 2;
	const static int DEMO_C_DIMENSION = 1;
	static Position START;
	static FRotator INIT_PLAYER_ROTATION;
	static FVector INIT_PLAYER_FORWARD;
	static FString MAIN_LEVEL_NAME;
	const static int DEMO_SEED = 10000;
	const static int MIN_DEMO_COUNT = 8;
	const static int MAX_DEMO_COUNT = 12;
	const static int TUTORIAL_SEED = 20000;
	const static int MIN_TUTORIAL_COUNT = 8;
	const static int MAX_TUTORIAL_COUNT = 12;

	static AMazeBuild* _mazeBuild;
	static USkyLightComponent* _skyLightComponent;
	static FRotator _playerRotator;
	static FVector _playerForward;
	static AController* _playerController;
	static Position _playerPosition;
	static int moveCount;
	static FRotator oldRotation;
	static FRotator newRotation;
	static FVector oldPositionXYZ;
	static FVector newPositionXYZ;
	static Position newPosition;
	static Direction dimensionTransitionDirection;
	static bool isDimensionTransition;
	static float lastAnimationTime;

	static float _animationDuration;
	constexpr static float WIN_DISPLAY_TIME = 6.0;
	static int _depth;
	static int _height;
	static int _width;
	static int _a_size;
	static int _b_size;
	static int _c_size;
	static int _save_depth;
	static int _save_height;
	static int _save_width;
	static int _save_a_size;
	static int _save_b_size;
	static int _save_c_size;
	static float _cellSize;
	static bool _initializing;
	static bool _win_shown;
	static bool _is_tutorial;
	static bool _is_demo;
	static bool _is_tooltip;
};
