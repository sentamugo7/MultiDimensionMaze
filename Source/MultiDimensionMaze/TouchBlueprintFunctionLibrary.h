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
	UFUNCTION(BlueprintCallable, Category = "Touch")
		static bool isPlayerMoving();

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
		static bool hasUPlus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasUMinus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasVPlus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasVMinus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasWPlus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasWMinus();

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
		static void clickUPlus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void clickUMinus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void clickVPlus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void clickVMinus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void clickWPlus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static void clickWMinus();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static FString getPlayerPositionUDisplay();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static FString getPlayerPositionVDisplay();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static FString getPlayerPositionWDisplay();
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
		static int getPlayerPositionU();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static int getPlayerPositionV();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static int getPlayerPositionW();
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
		static int getUSize();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static int getVSize();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static int getWSize();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static int getXSize();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static int getYSize();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static int getZSize();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static int getDimensionSize(int dimensionIndex);
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static void updateSettings(int uSize, int vSize, int wSize, int xSize, int ySize, int zSize);
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static void initMaze();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static void changeWall();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static void newMap(AMazeBuild* mazeBuild, APlayerController* playerController, ASkyLight* skyLight);
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static bool isWin();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "State")
		static void setTutorial(bool tutorial);
	UFUNCTION(BlueprintCallable, Category = "State")
		static bool isTutorial();
	UFUNCTION(BlueprintCallable, Category = "State")
		static void setDemo(bool demo);
	UFUNCTION(BlueprintCallable, Category = "State")
		static bool isDemo();
	UFUNCTION(BlueprintCallable, Category = "State")
		static void resetState();

	UFUNCTION(BlueprintPure, Category = "Meta")
		static FString getProjectVersion();

	static void setPlayer(ACharacter* player);
	static void rotatePlayer(FRotator rotator);
	static void movePlayer(bool isPress);
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
	const static int DEMO_U_DIMENSION = 1;
	const static int DEMO_V_DIMENSION = 1;
	const static int DEMO_W_DIMENSION = 2;
	const static int DEMO_X_DIMENSION = 3;
	const static int DEMO_Y_DIMENSION = 3;
	const static int DEMO_Z_DIMENSION = 2;
	const static int MAX_RAND_SEED = 1000000;
	const static int RAND_SEED_COUNT = 1000;
	static int RAND_SEEDS[RAND_SEED_COUNT];
	static Position START;
	static FRotator INIT_PLAYER_ROTATION;
	static FVector INIT_PLAYER_FORWARD;
	static FString MAIN_LEVEL_NAME;

	static AMazeBuild* _mazeBuild;
	static USkyLightComponent* _skyLightComponent;
	static bool _playerMoving;
	static FRotator _playerRotator;
	static FVector _playerForward;
	static AController* _playerController;
	static Position _playerPosition;

	constexpr static float WIN_DISPLAY_TIME = 6.0;
	static int randSeedIndex;
	static int _u_size;
	static int _v_size;
	static int _w_size;
	static int _depth;
	static int _height;
	static int _width;
	static int _save_u_size;
	static int _save_v_size;
	static int _save_w_size;
	static int _save_depth;
	static int _save_height;
	static int _save_width;
	static float _playerSpeed;
	static float _cellSize;
	static bool _win_shown;
	static bool _is_tutorial;
	static bool _is_demo;
};
