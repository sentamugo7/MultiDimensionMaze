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
		static void touchScreen(bool isPress);

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

	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasUp();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasDown();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasLeft();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasRight();
	UFUNCTION(BlueprintCallable, Category = "Dim")
		static bool hasReverse();
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
		static void turnPlayerReverse(bool isPress);

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
		static void updateSettings(int uSize, int vSize, int wSize, int xSize, int ySize, int zSize);
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static void changeWall();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static FString getDebugText();
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static void newMap(AMazeBuild* mazeBuild, APlayerController* playerController, ASkyLight* skyLight);
	UFUNCTION(BlueprintCallable, Category = "Settings")
		static void Tick(float DeltaTime);

	static void setPlayer(ACharacter* player);
	static void rotatePlayer(FRotator rotator);
	static void movePlayer(bool isPress);
	static void ShowWin();
	static void MovePlayerTo(Position pos);
	static FVector PLAYER_UP;
	static FVector PLAYER_FORWARD[];
	static ACharacter* _player;

private:
	static void initMaze();
	const static int DEFAULT_DIMENSION_SIZE = 3;
	constexpr static float DEFAULT_PLAYER_SPEED = 0.2;
	constexpr static float DEFAULT_CELL_SIZE = 1.0;
	constexpr static float WIN_DISPLAY_TIME = 5.0;
	static Position START;
	static FRotator INIT_PLAYER_ROTATION ;
	static FVector INIT_PLAYER_FORWARD;
	static FString MAIN_LEVEL_NAME;

	static AMazeBuild* _mazeBuild;
	static USkyLightComponent* _skyLightComponent;
	static bool _playerMoving;
	static FRotator _playerRotator;
	static FVector _playerForward;
	static AController* _playerController;
	static Position _playerPosition;
	static FString _debugText;

	static int _u_size;
	static int _v_size;
	static int _w_size;
	static int _depth;
	static int _height;
	static int _width;
	static float _playerSpeed;
	static float _cellSize;
	static float _win_showing_time;
	static bool _win_showing;
	static bool _win_shown;
};
