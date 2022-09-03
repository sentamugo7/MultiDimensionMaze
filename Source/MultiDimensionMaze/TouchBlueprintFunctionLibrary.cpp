#include "TouchBlueprintFunctionLibrary.h"

/* static variables */
FString UTouchBlueprintFunctionLibrary::MAIN_LEVEL_NAME = FString("Main");
Position UTouchBlueprintFunctionLibrary::START = Position(0, 0, 0, 0, 0, 0);
FRotator UTouchBlueprintFunctionLibrary::INIT_PLAYER_ROTATION = FRotator(0, -180, 0);
FVector UTouchBlueprintFunctionLibrary::INIT_PLAYER_FORWARD = FVector(-1, 0, 0);

AMazeBuild* UTouchBlueprintFunctionLibrary::_mazeBuild = NULL;
USkyLightComponent* UTouchBlueprintFunctionLibrary::_skyLightComponent = NULL;
ACharacter* UTouchBlueprintFunctionLibrary::_player;
FRotator UTouchBlueprintFunctionLibrary::_playerRotator = INIT_PLAYER_ROTATION;
AController* UTouchBlueprintFunctionLibrary::_playerController;
FVector UTouchBlueprintFunctionLibrary::_playerForward = INIT_PLAYER_FORWARD;
Position UTouchBlueprintFunctionLibrary::_playerPosition = START;
FRotator UTouchBlueprintFunctionLibrary::oldRotation;
FRotator UTouchBlueprintFunctionLibrary::newRotation;
FVector UTouchBlueprintFunctionLibrary::oldPositionXYZ;
FVector UTouchBlueprintFunctionLibrary::newPositionXYZ;
Position UTouchBlueprintFunctionLibrary::newPosition;
Direction UTouchBlueprintFunctionLibrary::dimensionTransitionDirection = NONE;
bool UTouchBlueprintFunctionLibrary::isDimensionTransition = false;
float UTouchBlueprintFunctionLibrary::lastAnimationTime = 0.0;

float UTouchBlueprintFunctionLibrary::_animationDuration = 0.0;
int UTouchBlueprintFunctionLibrary::moveCount;
FVector UTouchBlueprintFunctionLibrary::PLAYER_UP = FVector(0, 0, 1);

int UTouchBlueprintFunctionLibrary::_depth = INIT_Z_DIMENSION;
int UTouchBlueprintFunctionLibrary::_height = INIT_Y_DIMENSION;
int UTouchBlueprintFunctionLibrary::_width = INIT_X_DIMENSION;
int UTouchBlueprintFunctionLibrary::_a_size = INIT_A_DIMENSION;
int UTouchBlueprintFunctionLibrary::_b_size = INIT_B_DIMENSION;
int UTouchBlueprintFunctionLibrary::_c_size = INIT_C_DIMENSION;
int UTouchBlueprintFunctionLibrary::_difficulty = 0;
int UTouchBlueprintFunctionLibrary::_save_depth;
int UTouchBlueprintFunctionLibrary::_save_height;
int UTouchBlueprintFunctionLibrary::_save_width;
int UTouchBlueprintFunctionLibrary::_save_a_size;
int UTouchBlueprintFunctionLibrary::_save_b_size;
int UTouchBlueprintFunctionLibrary::_save_c_size;
int UTouchBlueprintFunctionLibrary::_save_difficulty = 0;
int UTouchBlueprintFunctionLibrary::_saveSettingsWidth = INIT_X_DIMENSION;
int UTouchBlueprintFunctionLibrary::_saveSettingsHeight = INIT_Y_DIMENSION;
int UTouchBlueprintFunctionLibrary::_saveSettingsDepth = INIT_Z_DIMENSION;
int UTouchBlueprintFunctionLibrary::_saveSettingsADimension = INIT_A_DIMENSION;
int UTouchBlueprintFunctionLibrary::_saveSettingsBDimension = INIT_B_DIMENSION;
int UTouchBlueprintFunctionLibrary::_saveSettingsCDimension = INIT_C_DIMENSION;

float UTouchBlueprintFunctionLibrary::_cellSize = DEFAULT_CELL_SIZE;
bool UTouchBlueprintFunctionLibrary::_is_tutorial = false;
bool UTouchBlueprintFunctionLibrary::_is_demo = false;
bool UTouchBlueprintFunctionLibrary::_is_tooltip = false;
bool UTouchBlueprintFunctionLibrary::_initializing = false;
bool UTouchBlueprintFunctionLibrary::_win_shown = false;

FVector UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[12] = { FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,1), FVector(0,0,-1), FVector(0,1,0), FVector(0,-1,0), FVector(-1,0,0), FVector(1,0,0) };

static const FString DIRECTION_HINT[] =
{ FString("hint_a_plus"), FString("hint_a_minus"), FString("hint_b_plus"), FString("hint_b_minus"), FString("hint_c_plus"), FString("hint_c_minus"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank") };

// UE_LOG(LogTemp, Warning, TEXT("UTouchBlueprintFunctionLibrary::movePlayer isPress = %i"), isPress);

/**
 * if the current Cell that the player is in has passage in the up direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasUp() {
    return !isUp() &&
        (
            ((_playerForward == PLAYER_FORWARD[SOUTH] || _playerForward == PLAYER_FORWARD[NORTH] || _playerForward == PLAYER_FORWARD[EAST] || _playerForward == PLAYER_FORWARD[WEST]) && _mazeBuild->GetMaze().getPassage(_playerPosition, UP_)) ||
            (_playerForward == PLAYER_FORWARD[DOWN_] && ((_playerRotator.Roll == 180 && _mazeBuild->GetMaze().getPassage(_playerPosition, WEST)) || (_playerRotator.Roll == 90 && _mazeBuild->GetMaze().getPassage(_playerPosition, SOUTH)) || (_playerRotator.Roll == 0 && _mazeBuild->GetMaze().getPassage(_playerPosition, EAST)) || (_playerRotator.Roll == 270 && _mazeBuild->GetMaze().getPassage(_playerPosition, NORTH))))
            )
        ;
}

/**
 * if the current Cell that the player is in has passage in the down direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasDown() {
    return !isDown() &&
        (
            ((_playerForward == PLAYER_FORWARD[SOUTH] || _playerForward == PLAYER_FORWARD[NORTH] || _playerForward == PLAYER_FORWARD[EAST] || _playerForward == PLAYER_FORWARD[WEST]) && _mazeBuild->GetMaze().getPassage(_playerPosition, DOWN_)) ||
            (_playerForward == PLAYER_FORWARD[UP_] && ((_playerRotator.Roll == 180 && _mazeBuild->GetMaze().getPassage(_playerPosition, WEST)) || (_playerRotator.Roll == 90 && _mazeBuild->GetMaze().getPassage(_playerPosition, SOUTH)) || (_playerRotator.Roll == 0 && _mazeBuild->GetMaze().getPassage(_playerPosition, EAST)) || (_playerRotator.Roll == 270 && _mazeBuild->GetMaze().getPassage(_playerPosition, NORTH))))
            )
        ;
}

/**
 * if the current Cell that the player is in has passage in the left direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasLeft() {
    return
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[SOUTH] && _mazeBuild->GetMaze().getPassage(_playerPosition, WEST)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[NORTH] && _mazeBuild->GetMaze().getPassage(_playerPosition, EAST)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[EAST] && _mazeBuild->GetMaze().getPassage(_playerPosition, SOUTH)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[WEST] && _mazeBuild->GetMaze().getPassage(_playerPosition, NORTH))
        ;
}

/**
 * if the current Cell that the player is in has passage in the right direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasRight() {
    return
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[SOUTH] && _mazeBuild->GetMaze().getPassage(_playerPosition, EAST)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[NORTH] && _mazeBuild->GetMaze().getPassage(_playerPosition, WEST)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[EAST] && _mazeBuild->GetMaze().getPassage(_playerPosition, NORTH)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[WEST] && _mazeBuild->GetMaze().getPassage(_playerPosition, SOUTH))
        ;
}

/**
 * if the current Cell that the player is in has passage in the forward direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasForward() {
    return
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[UP_] && _mazeBuild->GetMaze().getPassage(_playerPosition, UP_)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[DOWN_] && _mazeBuild->GetMaze().getPassage(_playerPosition, DOWN_)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[SOUTH] && _mazeBuild->GetMaze().getPassage(_playerPosition, SOUTH)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[NORTH] && _mazeBuild->GetMaze().getPassage(_playerPosition, NORTH)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[EAST] && _mazeBuild->GetMaze().getPassage(_playerPosition, EAST)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[WEST] && _mazeBuild->GetMaze().getPassage(_playerPosition, WEST))
        ;
}

/**
 * if the current Cell that the player is in has passage in the reverse direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasReverse() {
    return
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[UP_] && _mazeBuild->GetMaze().getPassage(_playerPosition, DOWN_)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[DOWN_] && _mazeBuild->GetMaze().getPassage(_playerPosition, UP_)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[SOUTH] && _mazeBuild->GetMaze().getPassage(_playerPosition, NORTH)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[NORTH] && _mazeBuild->GetMaze().getPassage(_playerPosition, SOUTH)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[EAST] && _mazeBuild->GetMaze().getPassage(_playerPosition, WEST)) ||
        (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[WEST] && _mazeBuild->GetMaze().getPassage(_playerPosition, EAST))
        ;
}

/**
 * if the current Cell that the player is in has passage in the a plus direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasAPlus() {
    return _mazeBuild->GetMaze().getPassage(_playerPosition, A_PLUS);
}

/**
 * if the current Cell that the player is in has passage in the a minus direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasAMinus() {
    return _mazeBuild->GetMaze().getPassage(_playerPosition, A_MINUS);
}

/**
 * if the current Cell that the player is in has passage in the b plus direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasBPlus() {
    return _mazeBuild->GetMaze().getPassage(_playerPosition, B_PLUS);
}

/**
 * if the current Cell that the player is in has passage in the b minus direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasBMinus() {
    return _mazeBuild->GetMaze().getPassage(_playerPosition, B_MINUS);
}

/**
 * if the current Cell that the player is in has passage in the c plus direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasCPlus() {
    return _mazeBuild->GetMaze().getPassage(_playerPosition, C_PLUS);
}

/**
 * if the current Cell that the player is in has passage in the c minus direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasCMinus() {
    return _mazeBuild->GetMaze().getPassage(_playerPosition, C_MINUS);
}

/**
 * if the player is going in the up direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::isUp() {
    return _playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[UP_];
}

/**
 * if the player is going in the up direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::isDown() {
    return _playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[DOWN_];
}

/**
 * user presses/unpresses up button
 *
 * @param {bool} isPress
 */
void UTouchBlueprintFunctionLibrary::turnPlayerUp(bool isPress) {
    if (isPress && _playerForward != UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[UP_]) {
        startRotatePlayer(FRotator(+90, 0, 0));
    }
}

/**
 * user presses/unpresses down button
 *
 * @param {bool} isPress
 */
void UTouchBlueprintFunctionLibrary::turnPlayerDown(bool isPress) {
    if (isPress && _playerForward != UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[DOWN_]) {
        startRotatePlayer(FRotator(-90, 0, 0));
    }
}

/**
 * user presses/unpresses left button
 *
 * @param {bool} isPress
 */
void UTouchBlueprintFunctionLibrary::turnPlayerLeft(bool isPress) {
    if (isPress) {
        startRotatePlayer(FRotator(0, -90, 0));
    }
}

/**
 * user presses/unpresses right button
 *
 * @param {bool} isPress
 */
void UTouchBlueprintFunctionLibrary::turnPlayerRight(bool isPress) {
    if (isPress) {
        startRotatePlayer(FRotator(0, +90, 0));
    }
}

/**
 * user presses/unpresses reverse button
 *
 * @param {bool} isPress
 */
void UTouchBlueprintFunctionLibrary::turnPlayerReverse(bool isPress) {
    if (isPress) {
        if (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[UP_]) {
            startRotatePlayer(FRotator(-180, 0, 0));
        }
        else if (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[DOWN_]) {
            startRotatePlayer(FRotator(+180, 0, 0));
        }
        else {
            startRotatePlayer(FRotator(0, +180, 0));
        }
    }
}

/**
 * user presses/unpresses forward button
 *
 * @param {bool} isPress
 */
void UTouchBlueprintFunctionLibrary::goPlayerForward(bool isPress) {
    if (isPress && hasForward()) {
        startPlayerForward(_playerPosition.neighbor(getPlayerForwardDirection()));
        moveCount++;
    }
}

/**
 * player clicks on the a plus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickAPlus() {
    startPlayerDimension(A_PLUS);
    moveCount++;
}

/**
 * player clicks on the a minus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickAMinus() {
    startPlayerDimension(A_MINUS);
    moveCount++;
}

/**
 * player clicks on the b plus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickBPlus() {
    startPlayerDimension(B_PLUS);
    moveCount++;
}

/**
 * player clicks on the b minus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickBMinus() {
    startPlayerDimension(B_MINUS);
    moveCount++;
}

/**
 * player clicks on the c plus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickCPlus() {
    startPlayerDimension(C_PLUS);
    moveCount++;
}

/**
 * player clicks on the c minus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickCMinus() {
    startPlayerDimension(C_MINUS);
    moveCount++;
}

/**
 * readable current player position a dimension
 *
 * @return {FString}
 */
FString UTouchBlueprintFunctionLibrary::getPlayerPositionADisplay() {
    return _mazeBuild->GetMaze().getASize()== 1 ? "" : FString::Printf(TEXT("a:%i/%i"), _playerPosition.getA() + 1, _mazeBuild->GetMaze().getASize());
}

/**
 * readable current player position b dimension
 *
 * @return {FString}
 */
FString UTouchBlueprintFunctionLibrary::getPlayerPositionBDisplay() {
    return _mazeBuild->GetMaze().getBSize() == 1 ? "" : FString::Printf(TEXT("b:%i/%i"), _playerPosition.getB() + 1, _mazeBuild->GetMaze().getBSize());
}

/**
 * readable current player position c dimension
 *
 * @return {FString}
 */
FString UTouchBlueprintFunctionLibrary::getPlayerPositionCDisplay() {
    return _mazeBuild->GetMaze().getCSize() == 1 ? "" : FString::Printf(TEXT("c:%i/%i"), _playerPosition.getC() + 1, _mazeBuild->GetMaze().getCSize());
}

/**
 * readable current player position x dimension
 *
 * @return {FString}
 */
FString UTouchBlueprintFunctionLibrary::getPlayerPositionXDisplay() {
    return _mazeBuild->GetMaze().getWidth() == 1 ? "" : FString::Printf(TEXT("x:%i/%i"), _playerPosition.getX() + 1, _mazeBuild->GetMaze().getWidth());
}

/**
 * readable current player position y dimension
 *
 * @return {FString}
 */
FString UTouchBlueprintFunctionLibrary::getPlayerPositionYDisplay() {
    return _mazeBuild->GetMaze().getHeight() == 1 ? "" : FString::Printf(TEXT("y:%i/%i"), _playerPosition.getY() + 1, _mazeBuild->GetMaze().getHeight());
}

/**
 * readable current player position z dimension
 *
 * @return {FString}
 */
FString UTouchBlueprintFunctionLibrary::getPlayerPositionZDisplay() {
    return _mazeBuild->GetMaze().getDepth() == 1 ? "" : FString::Printf(TEXT("z:%i/%i"), _playerPosition.getZ() + 1, _mazeBuild->GetMaze().getDepth());
}

/**
 * return the current hint direction (icon name)
 *
 * @return {FString}
 */
FString UTouchBlueprintFunctionLibrary::getHint() {
    Direction hintDir = _mazeBuild->GetMaze().getHintAt(_playerPosition);
    if (_win_shown) {
        return FString("hint_win");
    }
    if (_playerForward == PLAYER_FORWARD[hintDir]) {
        return FString("hint_forward");
    }
    if (_playerForward == PLAYER_FORWARD[dirs::opposite(hintDir)]) {
        return FString("hint_reverse");
    }
    if (_playerForward == PLAYER_FORWARD[UP_]) {
        return FString("hint_down");
    }
    if (_playerForward == PLAYER_FORWARD[DOWN_]) {
        return FString("hint_up");
    }
    if (hintDir == UP_) {
        return FString("hint_up");
    }
    if (hintDir == DOWN_) {
        return FString("hint_down");
    }
    if (FRotator(0, +90, 0).RotateVector(_playerForward).Equals(PLAYER_FORWARD[hintDir])) {
        return FString("hint_right");
    }
    if (FRotator(0, -90, 0).RotateVector(_playerForward).Equals(PLAYER_FORWARD[hintDir])) {
        return FString("hint_left");
    }
    return DIRECTION_HINT[hintDir];
}
/**
 * return the current orientation (icon name)
 *
 * @return {FString}
 */
FString UTouchBlueprintFunctionLibrary::getOrientation() {
    if (_playerForward == PLAYER_FORWARD[Direction::SOUTH]) {
        return FString("orientation_south");
    }
    if (_playerForward == PLAYER_FORWARD[Direction::EAST]) {
        return FString("orientation_east");
    }
    if (_playerForward == PLAYER_FORWARD[Direction::NORTH]) {
        return FString("orientation_north");
    }
    if (_playerForward == PLAYER_FORWARD[Direction::WEST]) {
        return FString("orientation_west");
    }
    int viewYaw = (int)(360 + round(_player->GetViewRotation().Yaw)) % 360;
    if (_playerForward == PLAYER_FORWARD[Direction::DOWN_]) {
        switch (viewYaw) {
        case 0:
            return FString("orientation_down_west");
        case 90:
            return FString("orientation_down_south");
        case 180:
            return FString("orientation_down_east");
        case 270:
            return FString("orientation_down_north");
        default:
            UE_LOG(LogTemp, Warning, TEXT("UTouchBlueprintFunctionLibrary::getOrientation _playerForward == PLAYER_FORWARD[Direction::DOWN_], viewYaw = %i"), viewYaw); /////
            return FString("dimensions");
        }
    }
    if (_playerForward == PLAYER_FORWARD[Direction::UP_]) {
        switch (viewYaw) {
        case 0:
            return FString("orientation_up_west");
        case 90:
            return FString("orientation_up_south");
        case 180:
            return FString("orientation_up_east");
        case 270:
            return FString("orientation_up_north");
        default:
            UE_LOG(LogTemp, Warning, TEXT("UTouchBlueprintFunctionLibrary::getOrientation _playerForward == PLAYER_FORWARD[Direction::UP_], viewYaw = %i"), viewYaw); /////
            return FString("dimensions");
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("UTouchBlueprintFunctionLibrary::getOrientation _playerForward == [%f,%f,%f], viewYaw = %i"), _playerForward.X, _playerForward.Y, _playerForward.Z, viewYaw); /////
    return FString("dimensions");
}

/**
 * current player position u dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getPlayerPositionA() {
    return _playerPosition.getA();
}

/**
 * current player position v dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getPlayerPositionB() {
    return _playerPosition.getB();
}

/**
 * current player position w dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getPlayerPositionC() {
    return _playerPosition.getC();
}

/**
 * current player position x dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getPlayerPositionX() {
    return _playerPosition.getX();
}

/**
 * current player position y dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getPlayerPositionY() {
    return _playerPosition.getY();
}

/**
 * current player position z dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getPlayerPositionZ() {
    return _playerPosition.getZ();
}

/**
 * current player position
 *
 * @return {FVector}
 */
FVector UTouchBlueprintFunctionLibrary::getPlayerPosition() {
    return FVector::LeftVector;
}

/**
 * current player forward
 *
 * @return {FVector}
 */
FVector UTouchBlueprintFunctionLibrary::getPlayerForward() {
    return _playerForward;
}

/**
 * current player rotation pitch
 *
 * @return {float}
 */
float UTouchBlueprintFunctionLibrary::getPlayerRotationPitch() {
    return UTouchBlueprintFunctionLibrary::_player->GetActorRotation().Pitch;
}

/**
 * current player rotation yaw
 *
 * @return {float}
 */
float UTouchBlueprintFunctionLibrary::getPlayerRotationYaw() {
    return UTouchBlueprintFunctionLibrary::_player->GetActorRotation().Yaw;
}

/**
 * current player rotation roll
 *
 * @return {float}
 */
float UTouchBlueprintFunctionLibrary::getPlayerRotationRoll() {
    return UTouchBlueprintFunctionLibrary::_player->GetActorRotation().Roll;
}

/**
 * initialize the Maze
 *
 */
void UTouchBlueprintFunctionLibrary::initMaze() {
    _initializing = true;
    int counter = 0;
    if (_is_demo) {
        srand(DEMO_SEED);
        do {
            _mazeBuild->NewMaze(_a_size, _b_size, _c_size, _depth, _height, _width);
            counter++;
        } while ((_mazeBuild->GetMaze().solutionCount() < MIN_DEMO_COUNT || _mazeBuild->GetMaze().solutionCount() > MAX_DEMO_COUNT || _mazeBuild->GetMaze().getLastSolution() < UP_ || _mazeBuild->GetMaze().getLastSolution() > WEST) && (counter < INIT_MAZE_MAX));
    }
    else if (_is_tutorial) {
        srand(TUTORIAL_SEED);
        do {
            _mazeBuild->NewMaze(_a_size, _b_size, _c_size, _depth, _height, _width);
            counter++;
        } while ((_mazeBuild->GetMaze().solutionCount() < MIN_TUTORIAL_COUNT || _mazeBuild->GetMaze().solutionCount() > MAX_TUTORIAL_COUNT || _mazeBuild->GetMaze().getLastSolution() < UP_ || _mazeBuild->GetMaze().getLastSolution() > WEST) && (counter < INIT_MAZE_MAX));
    } else {
        srand(time(NULL));
        srand(rand());
        srand(rand());
        double difficultyCalc;
        do {
            _mazeBuild->NewMaze(_a_size, _b_size, _c_size, _depth, _height, _width);
            counter++;
            difficultyCalc = (double)_mazeBuild->GetMaze().solutionCount() / (double)_mazeBuild->GetMaze().basisCount();
/////UE_LOG(LogTemp, Warning, TEXT("UTouchBlueprintFunctionLibrary::initMaze 1 solutionCount() = %i, basisCount() = %i, difficultyCalc = %f, _difficulty = %i, MIN_DIFFICULTY[_difficulty]=%f, MAX_DIFFICULTY[_difficulty] = %f, counter = %i"), _mazeBuild->GetMaze().solutionCount(), _mazeBuild->GetMaze().basisCount(), difficultyCalc, _difficulty, MIN_DIFFICULTY[_difficulty], MAX_DIFFICULTY[_difficulty], counter);
        } while ((difficultyCalc <= MIN_DIFFICULTY[_difficulty] - DIFFICULTY_BUFFER || difficultyCalc >= MAX_DIFFICULTY[_difficulty] + DIFFICULTY_BUFFER) && (counter < INIT_MAZE_MAX));
    }
    if (counter >=INIT_MAZE_MAX) {
        UE_LOG(LogTemp, Warning, TEXT("UTouchBlueprintFunctionLibrary::initMaze 2 counter = %i"), counter);
    }
    Maze maze = _mazeBuild->GetMaze();
    _playerRotator = INIT_PLAYER_ROTATION;
    _playerForward = INIT_PLAYER_FORWARD;
    _mazeBuild->startMaze(_playerRotator);
    MovePlayerTo(START);
    _win_shown = false;
    _playerController->SetControlRotation(INIT_PLAYER_ROTATION);
    moveCount = 0;
    _initializing = false;
}

/**
 * new UE4 map, pass map data as parms
 *
 * @param {AMazeBuild*} mazeBuild
 * @param {APlayerController*} playerController
 * @param {ASkyLight*} skyLight
 * @param {float*} animationDuration
 */
void UTouchBlueprintFunctionLibrary::newMap(AMazeBuild* mazeBuild, APlayerController* playerController, ASkyLight* skyLight, float animationDuration) {
    _mazeBuild = mazeBuild;
    APawn* pawn = playerController->GetPawn();
    ACharacter* player = static_cast<ACharacter*>(pawn);
    setPlayer(player);
    _skyLightComponent = skyLight->GetLightComponent();
    _animationDuration = animationDuration;
    initMaze();
}

/**
 * Saves and initializes the player
 *
 * @param {ACharacter*} player
 */
void UTouchBlueprintFunctionLibrary::setPlayer(ACharacter* player) {
    _player = player;
    if (_player == NULL) {
        UE_LOG(LogTemp, Log, TEXT(">>>>>>>> UTouchBlueprintFunctionLibrary::setPlayer FAIL _player == NULL !!!!!"));
        return;
    }
    _player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    player->bUseControllerRotationPitch = false;
    player->bUseControllerRotationRoll = false;
    player->bUseControllerRotationYaw = false;
    _playerController = _player->GetController();
    _player->SetActorLocation(_mazeBuild->positionToLocation(_playerPosition));
    _playerController->SetControlRotation(_playerRotator);
    FVector tempForward = FRotationMatrix(_playerRotator).GetScaledAxis(EAxis::X);
    _playerForward = FVector((float)round(tempForward.X), (float)round(tempForward.Y), (float)round(tempForward.Z));
}

/**
 * start rotate the player in the specified rotation.
 *
 * @param {FRotator} rotator
 */
void UTouchBlueprintFunctionLibrary::startRotatePlayer(FRotator rotator) {
    FRotator tempRotation = _playerController->GetControlRotation();
    oldRotation = FRotator(tempRotation.Pitch, tempRotation.Yaw, tempRotation.Roll);
    newRotation = tempRotation.Add(rotator.Pitch, rotator.Yaw, rotator.Roll);
}

/**
 * finish rotate the player in the specified rotation.
 *
 */
void UTouchBlueprintFunctionLibrary::finishRotatePlayer() {
    _playerController->SetControlRotation(newRotation);
    FVector tempForward = FRotationMatrix(newRotation).GetScaledAxis(EAxis::X);
    _playerForward = FVector((float)round(tempForward.X), (float)round(tempForward.Y), (float)round(tempForward.Z));
    _playerRotator = FRotator((float)(((int)round(newRotation.Pitch)) % 360), (float)(((int)round(newRotation.Yaw)) % 360), (float)(((int)round(newRotation.Roll)) % 360));
    Direction dir = A_PLUS;
    if (_playerForward.X == +1) {
        dir = EAST;
    }
    else if (_playerForward.X == -1) {
        dir = WEST;
    }
    else if (_playerForward.Y == +1) {
        dir = SOUTH;
    }
    else if (_playerForward.Y == -1) {
        dir = NORTH;
    }
    else if (_playerForward.Z == +1) {
        dir = UP_;
    }
    else if (_playerForward.Z == -1) {
        dir = DOWN_;
    }
    _mazeBuild->ChangeDirection(dir, _playerRotator);
    oldRotation = newRotation;
}

/**
 * start the player forward to the specified position.
 *
 * @param {FRotator} rotator
 */
void UTouchBlueprintFunctionLibrary::startPlayerForward(Position position) {
    newPosition = position;
    oldPositionXYZ = FVector(getPlayerPositionX(), getPlayerPositionY(), getPlayerPositionZ());
    newPositionXYZ = FVector(position.getX(), position.getY(), position.getZ());
}

/**
 * finish the player forward to the specified position.
 *
 */
void UTouchBlueprintFunctionLibrary::finishPlayerForward() {
    _player->SetActorLocation(_mazeBuild->positionToLocation(newPosition));
    _playerPosition = newPosition;
    oldPositionXYZ = newPositionXYZ;
}

/**
 * start the player in the specified dimension.
 *
 * @param {Direction} direction
 */
void UTouchBlueprintFunctionLibrary::startPlayerDimension(Direction direction) {
    dimensionTransitionDirection = direction;
    isDimensionTransition = true;
}

/**
 * switch the player to the specified dimension.
 *
 */
void UTouchBlueprintFunctionLibrary::switchPlayerDimension() {
    _playerPosition = _mazeBuild->MoveDimension(_playerPosition, dimensionTransitionDirection);
    dimensionTransitionDirection = NONE;
}

/**
 * maze size a dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getASize() {
    return _a_size;
}

/**
 * maze size b dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getBSize() {
    return _b_size;
}

/**
 * maze size c dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getCSize() {
    return _c_size;
}

/**
 * maze size x dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getXSize() {
    return _width;
}

/**
 * maze size y dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getYSize() {
    return _height;
}

/**
 * maze size z dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getZSize() {
    return _depth;
}

/**
 * get the selected difficulty (easy, medium, hard, random)
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getDifficulty() {
    return _difficulty;
}

/**
 * get maze size in the specified dimension
 *
 * @param {int} dimensionIndex
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getDimensionSize(int dimensionIndex) {
    switch (dimensionIndex) { 
    case 0:
        return _width;
    case 1:
        return _height;
    case 2:
        return _depth;
    case 3:
        return _a_size;
    case 4:
        return _b_size;
    case 5:
        return _c_size;
    default:
        return -1;
    }
}


void UTouchBlueprintFunctionLibrary::setDifficultyRange() {
    double mazeTest[MAZE_TEST_COUNT];
    srand(time(NULL));
    srand(rand());
    srand(rand());
    double difficultyCalc;
    for (int mazeLoop = 0; mazeLoop < MAZE_TEST_COUNT; mazeLoop++){
        _mazeBuild->NewMaze(_a_size, _b_size, _c_size, _depth, _height, _width);
        difficultyCalc = (double)_mazeBuild->GetMaze().solutionCount() / (double)_mazeBuild->GetMaze().basisCount();
        mazeTest[mazeLoop] = difficultyCalc;
/////        UE_LOG(LogTemp, Warning, TEXT("UTouchBlueprintFunctionLibrary::setDifficultyRange 1 mazeLoop = %i, difficultyCalc = %f"), mazeLoop, difficultyCalc);
    }
    std::sort(std::begin(mazeTest), std::end(mazeTest));
    int easyMedium = floor((double)MAZE_TEST_COUNT / 12.0);
    int mediumHard = floor((double)MAZE_TEST_COUNT / 2.0);
/////    UE_LOG(LogTemp, Warning, TEXT("UTouchBlueprintFunctionLibrary::setDifficultyRange 2 easyMedium = %i ,%f, mediumHard = %i, %f"), easyMedium, mazeTest[easyMedium], mediumHard, mazeTest[mediumHard]);
    MAX_DIFFICULTY[0] = mazeTest[easyMedium];
    MIN_DIFFICULTY[1] = mazeTest[easyMedium];
    MAX_DIFFICULTY[1] = mazeTest[mediumHard];
    MIN_DIFFICULTY[2] = mazeTest[mediumHard];
}

/**
 * update user settings, dimension size in each dimension
 *
 * @param {int} xSize
 * @param {int} ySize
 * @param {int} zSize
 * @param {int} aSize
 * @param {int} bSize
 * @param {int} cSize
 * @param {int} difficulty
 */
void UTouchBlueprintFunctionLibrary::updateSettings(int xSize, int ySize, int zSize, int aSize, int bSize, int cSize, int difficulty) {
    _initializing = true;
    _width  = xSize;
    _height = ySize;
    _depth  = zSize;
    _a_size = aSize;
    _b_size = bSize;
    _c_size = cSize;
    _difficulty = difficulty;
    if (xSize != _saveSettingsWidth || ySize != _saveSettingsHeight || zSize != _saveSettingsDepth || aSize != _saveSettingsADimension || bSize != _saveSettingsBDimension || cSize != _saveSettingsCDimension) {
/////UE_LOG(LogTemp, Warning, TEXT("UTouchBlueprintFunctionLibrary::updateSettings 1 xSize = %i, _saveSettingsWidth = %i, ySize = %i, _saveSettingsHeight = %i, zSize = %i, _saveSettingsDepth = %i, aSize = %i, _saveSettingsADimension = %i, bSize = %i, _saveSettingsBDimension = %i, cSize = %i, _saveSettingsCDimension = %i"), xSize, _saveSettingsWidth, ySize, _saveSettingsHeight, zSize, _saveSettingsDepth, aSize, _saveSettingsADimension, bSize, _saveSettingsBDimension, cSize, _saveSettingsCDimension);
        setDifficultyRange();
        _saveSettingsWidth = xSize;
        _saveSettingsHeight = ySize;
        _saveSettingsDepth = zSize;
        _saveSettingsADimension = aSize;
        _saveSettingsBDimension = bSize;
        _saveSettingsCDimension = cSize;
    }
    initMaze();
    _initializing = false;
}

/**
 * initialize user settingsfor tutorial
 *
 */
void UTouchBlueprintFunctionLibrary::initTutorialSettings() {
    updateSettings(INIT_X_DIMENSION, INIT_Y_DIMENSION, INIT_Z_DIMENSION, INIT_A_DIMENSION, INIT_B_DIMENSION, INIT_C_DIMENSION, 0);
}

/**
 * change walls to new wall selecion
 *
 */
void UTouchBlueprintFunctionLibrary::changeWall() {
    _mazeBuild->changeWall();
}

/**
 * after win displayed, create new maze
 *
 */
void UTouchBlueprintFunctionLibrary::FinishWin() {
    _mazeBuild->endShowWin();
}

/**
 * Display that the player has won, reached the end Call.
 *
 */
void UTouchBlueprintFunctionLibrary::ShowWin() {
    _mazeBuild->playWin();
    FTimerHandle TimerHandle;
    _mazeBuild->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
        {
           FinishWin();
        }, WIN_DISPLAY_TIME, false);
}

Direction UTouchBlueprintFunctionLibrary::getPlayerForwardDirection() {
    if (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[UP_]) {
        return Direction::UP_;
    }
    else if (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[DOWN_]) {
        return Direction::DOWN_;
    }
    else if (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[SOUTH]) {
        return Direction::SOUTH;
    }
    else if (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[NORTH]) {
        return Direction::NORTH;
    }
    else if (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[EAST]) {
        return Direction::EAST;
    }
    else if (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[WEST]) {
        return Direction::WEST;
    }
    UE_LOG(LogTemp, Log, TEXT(">>>>>>>> UTouchBlueprintFunctionLibrary::getPlayerForwardDirection FAIL _playerForward == [%f,%f,%f] !!!!!"), _playerForward.X, _playerForward.Y, _playerForward.Z);
    return Direction::NONE;
}

/**
 * indicate if the player has won
 *
 * @return bool
 */
bool UTouchBlueprintFunctionLibrary::isWin() {
    return _win_shown;
}

// Called every frame while in MazePage
void UTouchBlueprintFunctionLibrary::MazeTick(float animationTime) {
    if (_win_shown || _initializing) {
        return;
    }
    if (animationTime > 0.0) {
        float animationPercent = ((_animationDuration - animationTime) / _animationDuration);
        if (oldRotation != newRotation) {
            FRotator tickRotation = oldRotation + animationPercent * (newRotation - oldRotation);
            _playerController->SetControlRotation(tickRotation);
        } else
        if (oldPositionXYZ != newPositionXYZ) {
            FVector tickPositionXYZ = oldPositionXYZ + animationPercent * (newPositionXYZ - oldPositionXYZ);
            _player->SetActorLocation(_mazeBuild->positionToLocation(Position(), tickPositionXYZ));
        } else
        if (dimensionTransitionDirection != NONE) {
            _mazeBuild->displayDimensionTransition(std::min(1.0, animationPercent * 2.0));
            if (animationPercent >= 0.5) {
                switchPlayerDimension();
            }
        } else
        if (isDimensionTransition) {
            _mazeBuild->displayDimensionTransition(std::min(1.0, (1 - animationPercent) * 2.0));
        }
    } else {
        isDimensionTransition = false;
        if (oldRotation != newRotation) {
            finishRotatePlayer();
        } else
            if (oldPositionXYZ != newPositionXYZ) {
                finishPlayerForward();
            }
    }
    _playerPosition = _mazeBuild->locationToPosition(_playerPosition.getA(), _playerPosition.getB(), _playerPosition.getC(), _player->GetActorLocation());
    if (_playerPosition == _mazeBuild->GetMaze().getEnd() && !_win_shown && !isDimensionTransition) {
        oldRotation = newRotation;
        oldPositionXYZ = newPositionXYZ;
        isDimensionTransition = false;
        dimensionTransitionDirection = NONE;
        _win_shown = true;
        ShowWin();
    }
    if (animationTime == 0.0 && lastAnimationTime > 0.0) {
        _mazeBuild->finishAnimation();
    }
    lastAnimationTime = animationTime;
}

/**
 * get the user score
 *
 * @return FString
 */
FString UTouchBlueprintFunctionLibrary::getScore() {
    return FString::FromInt(moveCount) + FString("/") + FString::FromInt(_mazeBuild->GetMaze().solutionCount());
}

/**
 * Setter for tutorial state
 *
 * @param {bool} tutorial
 */
void UTouchBlueprintFunctionLibrary::setTutorial(bool tutorial) {
    _is_tutorial  = tutorial;
    if (_is_tutorial) {
        _save_a_size = _a_size;
        _save_b_size = _b_size;
        _save_c_size = _c_size;
        _save_depth = _depth;
        _save_height = _height;
        _save_width = _width;
        _save_difficulty = _difficulty;
        updateSettings(INIT_X_DIMENSION, INIT_Y_DIMENSION, INIT_Z_DIMENSION, INIT_A_DIMENSION, INIT_B_DIMENSION, INIT_C_DIMENSION, 0);
    }
    else {
        updateSettings(_save_width, _save_height, _save_depth, _save_a_size, _save_b_size, _save_c_size, _save_difficulty);
    }
}

/**
 * Getter for tutorial state
 *
 * @return bool
 */
bool UTouchBlueprintFunctionLibrary::isTutorial() {
    return _is_tutorial;
}

/**
 * Setter for demo state
 *
 * @param {bool} demo
 */
void UTouchBlueprintFunctionLibrary::setDemo(bool demo) {
    _is_demo = demo;
    if (_is_demo) {
        _save_a_size = _a_size;
        _save_b_size = _b_size;
        _save_c_size = _c_size;
        _save_depth = _depth;
        _save_height = _height;
        _save_width = _width;
        _save_difficulty = _difficulty;
        updateSettings(DEMO_X_DIMENSION, DEMO_Y_DIMENSION, DEMO_Z_DIMENSION, DEMO_A_DIMENSION, DEMO_B_DIMENSION, DEMO_C_DIMENSION, 0);
    }
    else {
        updateSettings(_save_width, _save_height, _save_depth, _save_a_size, _save_b_size, _save_c_size, _save_difficulty);
    }
}

/**
 * Getter for demo state
 *
 * @return bool
 */
bool UTouchBlueprintFunctionLibrary::isDemo() {
    return _is_demo;
}

/**
 * Setter for tooltip state
 *
 * @param {bool} tooltip
 */
void UTouchBlueprintFunctionLibrary::setTooltip(bool tooltip) {
    _is_tooltip = tooltip;
}

/**
 * Getter for tooltip state
 *
 * @return bool
 */
bool UTouchBlueprintFunctionLibrary::isTooltip() {
    return _is_tooltip;
}

/**
 * reset demo/tutorial state
 *
 */
void UTouchBlueprintFunctionLibrary::resetState() {
    bool isTutorialOrDemo = isTutorial() || isDemo();
    _is_tutorial = false;
    _is_demo = false;
    _is_tooltip = false;
    if (isTutorialOrDemo) {
        updateSettings(_save_width, _save_height, _save_depth, _save_a_size, _save_b_size, _save_c_size, _save_difficulty);
    }
    else {
        initMaze();
    }
}

/**
 * return the product version
 *
 * @return {FString}
 */
FString UTouchBlueprintFunctionLibrary::getProjectVersion() {
    return GetDefault<UGeneralProjectSettings>()->ProjectVersion;
}
/**
 * Move the player to the specified Position
 *
 * @param {Position} pos
 */
void UTouchBlueprintFunctionLibrary::MovePlayerTo(Position pos) {
    _player->SetActorLocation(_mazeBuild->positionToLocation(pos));
    _playerPosition = pos;
}
