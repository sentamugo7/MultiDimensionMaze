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

float UTouchBlueprintFunctionLibrary::_animationDuration = 0.0;
int UTouchBlueprintFunctionLibrary::moveCount;
FVector UTouchBlueprintFunctionLibrary::PLAYER_UP = FVector(0, 0, 1);

int UTouchBlueprintFunctionLibrary::_u_size = 1;//DEFAULT_DIMENSION_SIZE;
int UTouchBlueprintFunctionLibrary::_v_size = 1;// DEFAULT_DIMENSION_SIZE;
int UTouchBlueprintFunctionLibrary::_w_size = 1;// DEFAULT_DIMENSION_SIZE;
int UTouchBlueprintFunctionLibrary::_depth = 1;
int UTouchBlueprintFunctionLibrary::_height = DEFAULT_DIMENSION_SIZE;
int UTouchBlueprintFunctionLibrary::_width = DEFAULT_DIMENSION_SIZE;
int UTouchBlueprintFunctionLibrary::_save_u_size;
int UTouchBlueprintFunctionLibrary::_save_v_size;
int UTouchBlueprintFunctionLibrary::_save_w_size;
int UTouchBlueprintFunctionLibrary::_save_depth;
int UTouchBlueprintFunctionLibrary::_save_height;
int UTouchBlueprintFunctionLibrary::_save_width;
float UTouchBlueprintFunctionLibrary::_cellSize = DEFAULT_CELL_SIZE;
bool UTouchBlueprintFunctionLibrary::_is_tutorial = false;
bool UTouchBlueprintFunctionLibrary::_is_demo = false;
bool UTouchBlueprintFunctionLibrary::_win_shown = false;
int UTouchBlueprintFunctionLibrary::RAND_SEEDS[RAND_SEED_COUNT];
int UTouchBlueprintFunctionLibrary::randSeedIndex = 0;

FVector UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[12] = { FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,1), FVector(0,0,-1), FVector(0,1,0), FVector(0,-1,0), FVector(-1,0,0), FVector(1,0,0) };

static const FString DIRECTION_HINT[] =
{ FString("hint_u_plus"), FString("hint_u_minus"), FString("hint_v_plus"), FString("hint_v_minus"), FString("hint_w_plus"), FString("hint_w_minus"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank") };

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
 * if the current Cell that the player is in has passage in the u plus direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasUPlus() {
    return _mazeBuild->GetMaze().getPassage(_playerPosition, U_PLUS);
}

/**
 * if the current Cell that the player is in has passage in the u minus direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasUMinus() {
    return _mazeBuild->GetMaze().getPassage(_playerPosition, U_MINUS);
}

/**
 * if the current Cell that the player is in has passage in the v plus direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasVPlus() {
    return _mazeBuild->GetMaze().getPassage(_playerPosition, V_PLUS);
}

/**
 * if the current Cell that the player is in has passage in the v minus direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasVMinus() {
    return _mazeBuild->GetMaze().getPassage(_playerPosition, V_MINUS);
}

/**
 * if the current Cell that the player is in has passage in the w plus direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasWPlus() {
    return _mazeBuild->GetMaze().getPassage(_playerPosition, W_PLUS);
}

/**
 * if the current Cell that the player is in has passage in the w minus direction
 *
 * @param {bool}
 */
bool UTouchBlueprintFunctionLibrary::hasWMinus() {
    return _mazeBuild->GetMaze().getPassage(_playerPosition, W_MINUS);
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
 * player clicks on the u plus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickUPlus() {
    startPlayerDimension(U_PLUS);
    moveCount++;
}

/**
 * player clicks on the u minus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickUMinus() {
    startPlayerDimension(U_MINUS);
    moveCount++;
}

/**
 * player clicks on the v plus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickVPlus() {
    startPlayerDimension(V_PLUS);
    moveCount++;
}

/**
 * player clicks on the v minus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickVMinus() {
    startPlayerDimension(V_MINUS);
    moveCount++;
}

/**
 * player clicks on the w plus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickWPlus() {
    startPlayerDimension(W_PLUS);
    moveCount++;
}

/**
 * player clicks on the w minus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickWMinus() {
    startPlayerDimension(W_MINUS);
    moveCount++;
}

/**
 * readable current player position u dimension
 *
 * @return {FString}
 */
FString UTouchBlueprintFunctionLibrary::getPlayerPositionUDisplay() {
    return _mazeBuild->GetMaze().getUSize()== 1 ? "" : FString::Printf(TEXT("u:%i/%i"), _playerPosition.getU() + 1, _mazeBuild->GetMaze().getUSize());
}

/**
 * readable current player position v dimension
 *
 * @return {FString}
 */
FString UTouchBlueprintFunctionLibrary::getPlayerPositionVDisplay() {
    return _mazeBuild->GetMaze().getVSize() == 1 ? "" : FString::Printf(TEXT("v:%i/%i"), _playerPosition.getV() + 1, _mazeBuild->GetMaze().getVSize());
}

/**
 * readable current player position w dimension
 *
 * @return {FString}
 */
FString UTouchBlueprintFunctionLibrary::getPlayerPositionWDisplay() {
    return _mazeBuild->GetMaze().getWSize() == 1 ? "" : FString::Printf(TEXT("w:%i/%i"), _playerPosition.getW() + 1, _mazeBuild->GetMaze().getWSize());
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
int UTouchBlueprintFunctionLibrary::getPlayerPositionU() {
    return _playerPosition.getU();
}

/**
 * current player position v dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getPlayerPositionV() {
    return _playerPosition.getV();
}

/**
 * current player position w dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getPlayerPositionW() {
    return _playerPosition.getW();
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
 * @param {int} seed - the random number seed input or zero for none
 */
void UTouchBlueprintFunctionLibrary::initMaze(int seed) {
    unsigned int randSeed;
    if (seed == 0) {
        randSeedIndex = (randSeedIndex + 1) % RAND_SEED_COUNT;
        randSeed = RAND_SEEDS[randSeedIndex];
    }
    else {
        randSeed = seed;
    }
    srand(randSeed);
    _mazeBuild->displayRandSeed(randSeed);
    _mazeBuild->NewMaze(_u_size, _v_size, _w_size, _depth, _height, _width);
    Maze maze = _mazeBuild->GetMaze();
    _mazeBuild->startMaze();
    _playerRotator = INIT_PLAYER_ROTATION;
    _playerForward = INIT_PLAYER_FORWARD;
    MovePlayerTo(START);
    _win_shown = false;
    _playerController->SetControlRotation(INIT_PLAYER_ROTATION);
    moveCount = 0;
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
    for (int loop = 0; loop < RAND_SEED_COUNT; loop++) {
        RAND_SEEDS[loop] = (abs(rand())) % MAX_RAND_SEED;
    }
    _mazeBuild = mazeBuild;
    APawn* pawn = playerController->GetPawn();
    ACharacter* player = static_cast<ACharacter*>(pawn);
    setPlayer(player);
    _skyLightComponent = skyLight->GetLightComponent();
    _animationDuration = animationDuration;
    initMaze(0);
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
    Direction dir = U_PLUS;
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
    _mazeBuild->ChangeDirection(dir);
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
 * maze size u dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getUSize() {
    return _u_size;
}

/**
 * maze size v dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getVSize() {
    return _v_size;
}

/**
 * maze size w dimension
 *
 * @return {int}
 */
int UTouchBlueprintFunctionLibrary::getWSize() {
    return _w_size;
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

int UTouchBlueprintFunctionLibrary::getDimensionSize(int dimensionIndex) {
    switch (dimensionIndex) { 
    case 0:
        return _u_size;
    case 1:
        return _v_size;
    case 2:
        return _w_size;
    case 3:
        return _width;
    case 4:
        return _height;
    case 5:
        return _depth;
    default:
        return -1;
    }
}

/**
 * update user settings, dimension size in each dimension
 *
 * @param {int} uSize
 * @param {int} vSize
 * @param {int} wSize
 * @param {int} xSize
 * @param {int} ySize
 * @param {int} zSize
 * @param {string} seed (blank for none)
 */
void UTouchBlueprintFunctionLibrary::updateSettings(int uSize, int vSize, int wSize, int xSize, int ySize, int zSize, int seed) {
    _u_size = uSize;
    _v_size = vSize;
    _w_size = wSize;
    _width  = xSize;
    _height = ySize;
    _depth  = zSize;
    initMaze(seed);
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
void UTouchBlueprintFunctionLibrary::MazeTick(float animationTime)
{
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
    _playerPosition = _mazeBuild->locationToPosition(_playerPosition.getU(), _playerPosition.getV(), _playerPosition.getW(), _player->GetActorLocation());
    if (_playerPosition == _mazeBuild->GetMaze().getEnd() && !_win_shown && !isDimensionTransition) {
        oldRotation = newRotation;
        oldPositionXYZ = newPositionXYZ;
        isDimensionTransition = false;
        dimensionTransitionDirection = NONE;
        _win_shown = true;
        ShowWin();
    }
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
        _save_u_size = _u_size;
        _save_v_size = _v_size;
        _save_w_size = _w_size;
        _save_depth = _depth;
        _save_height = _height;
        _save_width = _width;
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
        _save_u_size = _u_size;
        _save_v_size = _v_size;
        _save_w_size = _w_size;
        _save_depth = _depth;
        _save_height = _height;
        _save_width = _width;
        updateSettings(DEMO_U_DIMENSION, DEMO_V_DIMENSION, DEMO_W_DIMENSION, DEMO_X_DIMENSION, DEMO_Y_DIMENSION, DEMO_Z_DIMENSION, 0);
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
 * reset demo/tutorial state
 *
 */
void UTouchBlueprintFunctionLibrary::resetState() {
    if (isTutorial() || isDemo()) {
        updateSettings(_save_u_size, _save_v_size, _save_w_size, _save_width, _save_height, _save_depth, 0);
    }
    else {
        initMaze(0);
    }
    setTutorial(false);
    setDemo(false);
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
