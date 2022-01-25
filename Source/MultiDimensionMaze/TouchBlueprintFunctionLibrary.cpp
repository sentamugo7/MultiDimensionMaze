#include "TouchBlueprintFunctionLibrary.h"

/* static variables */
FString UTouchBlueprintFunctionLibrary::MAIN_LEVEL_NAME = FString("Main");
Position UTouchBlueprintFunctionLibrary::START = Position(0, 0, 0, 0, 0, 0);
FRotator UTouchBlueprintFunctionLibrary::INIT_PLAYER_ROTATION = FRotator(0, -180, 0);
FVector UTouchBlueprintFunctionLibrary::INIT_PLAYER_FORWARD = FVector(-1, 0, 0);

AMazeBuild* UTouchBlueprintFunctionLibrary::_mazeBuild = NULL;
USkyLightComponent* UTouchBlueprintFunctionLibrary::_skyLightComponent = NULL;
ACharacter* UTouchBlueprintFunctionLibrary::_player;
bool UTouchBlueprintFunctionLibrary::_playerMoving = false;
FRotator UTouchBlueprintFunctionLibrary::_playerRotator = INIT_PLAYER_ROTATION;
AController* UTouchBlueprintFunctionLibrary::_playerController;
FVector UTouchBlueprintFunctionLibrary::_playerForward = INIT_PLAYER_FORWARD;
Position UTouchBlueprintFunctionLibrary::_playerPosition = START;
FString UTouchBlueprintFunctionLibrary::_debugText;
FVector UTouchBlueprintFunctionLibrary::PLAYER_UP = FVector(0, 0, 1);

int UTouchBlueprintFunctionLibrary::_u_size = 1;//DEFAULT_DIMENSION_SIZE;
int UTouchBlueprintFunctionLibrary::_v_size = 1;// DEFAULT_DIMENSION_SIZE;
int UTouchBlueprintFunctionLibrary::_w_size = 1;// DEFAULT_DIMENSION_SIZE;
int UTouchBlueprintFunctionLibrary::_depth = DEFAULT_DIMENSION_SIZE;
int UTouchBlueprintFunctionLibrary::_height = DEFAULT_DIMENSION_SIZE;
int UTouchBlueprintFunctionLibrary::_width = DEFAULT_DIMENSION_SIZE;
float UTouchBlueprintFunctionLibrary::_playerSpeed = DEFAULT_PLAYER_SPEED;
float UTouchBlueprintFunctionLibrary::_cellSize = DEFAULT_CELL_SIZE;
bool UTouchBlueprintFunctionLibrary::_win_showing = false;
float UTouchBlueprintFunctionLibrary::_win_showing_time = 0.0;
bool UTouchBlueprintFunctionLibrary::_win_shown = false;

FVector UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[12] = { FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,0), FVector(0,0,1), FVector(0,0,-1), FVector(0,1,0), FVector(0,-1,0), FVector(-1,0,0), FVector(1,0,0) };

static const FString DIRECTION_HINT[] =
{ FString("hint_u_plus"), FString("hint_u_minus"), FString("hint_v_plus"), FString("hint_v_minus"), FString("hint_w_plus"), FString("hint_w_minus"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank"), FString("hint_blank") };

// UE_LOG(LogTemp, Warning, TEXT("UTouchBlueprintFunctionLibrary::movePlayer isPress = %i"), isPress);

/**
 * user presses/unpresses screen to go forward
 *
 * @param {bool} isPress
 */
void UTouchBlueprintFunctionLibrary::touchScreen(bool isPress) {
    movePlayer(isPress);
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
 * player clicks on the u plus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickUPlus() {
    _playerPosition = _mazeBuild->MoveDimension(_playerPosition, U_PLUS);
    _debugText += FString::Printf(TEXT(" MOVETO (%i,%i,%i,%i,%i,%i) ==== "), _playerPosition.getU(), _playerPosition.getV(), _playerPosition.getW(), _playerPosition.getZ(), _playerPosition.getY(), _playerPosition.getX());
}

/**
 * player clicks on the u minus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickUMinus() {
    _playerPosition = _mazeBuild->MoveDimension(_playerPosition, U_MINUS);
    _debugText += FString::Printf(TEXT(" MOVETO (%i,%i,%i,%i,%i,%i) ==== "), _playerPosition.getU(), _playerPosition.getV(), _playerPosition.getW(), _playerPosition.getZ(), _playerPosition.getY(), _playerPosition.getX());
}

/**
 * player clicks on the v plus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickVPlus() {
    _playerPosition = _mazeBuild->MoveDimension(_playerPosition, V_PLUS);
    _debugText += FString::Printf(TEXT(" MOVETO (%i,%i,%i,%i,%i,%i) ==== "), _playerPosition.getU(), _playerPosition.getV(), _playerPosition.getW(), _playerPosition.getZ(), _playerPosition.getY(), _playerPosition.getX());
}

/**
 * player clicks on the v minus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickVMinus() {
    _playerPosition = _mazeBuild->MoveDimension(_playerPosition, V_MINUS);
    _debugText += FString::Printf(TEXT(" MOVETO (%i,%i,%i,%i,%i,%i) ==== "), _playerPosition.getU(), _playerPosition.getV(), _playerPosition.getW(), _playerPosition.getZ(), _playerPosition.getY(), _playerPosition.getX());
}

/**
 * player clicks on the w plus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickWPlus() {
    _playerPosition = _mazeBuild->MoveDimension(_playerPosition, W_PLUS);
    _debugText += FString::Printf(TEXT(" MOVETO (%i,%i,%i,%i,%i,%i) ==== "), _playerPosition.getU(), _playerPosition.getV(), _playerPosition.getW(), _playerPosition.getZ(), _playerPosition.getY(), _playerPosition.getX());
}

/**
 * player clicks on the w minus button
 *
 */
void UTouchBlueprintFunctionLibrary::clickWMinus() {
    _playerPosition = _mazeBuild->MoveDimension(_playerPosition, W_MINUS);
    _debugText += FString::Printf(TEXT(" MOVETO (%i,%i,%i,%i,%i,%i) ==== "), _playerPosition.getU(), _playerPosition.getV(), _playerPosition.getW(), _playerPosition.getZ(), _playerPosition.getY(), _playerPosition.getX());
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
            return FString("dimensions");
        }
    }
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
 */
void UTouchBlueprintFunctionLibrary::initMaze() {
    _mazeBuild->NewMaze(_u_size, _v_size, _w_size, _depth, _height, _width);
    Maze maze = _mazeBuild->GetMaze();
    _debugText = FString();
    for (int uLoop = 0; uLoop < _u_size; uLoop++) {
   		for (int vLoop = 0; vLoop < _v_size; vLoop++) {
   			for (int wLoop = 0; wLoop < _w_size; wLoop++) {
                for (int zLoop = 0; zLoop <= _depth; zLoop++) {
                    for (int yLoop = 0; yLoop <= _height; yLoop++) {
                        for (int xLoop = 0; xLoop <= _width; xLoop++) {
                            Position pos = Position(uLoop, vLoop, wLoop, zLoop, yLoop, xLoop);
                            for (int dirLoop = 0; dirLoop < DIRECTION_COUNT; dirLoop++) {
                                Direction dir = static_cast<Direction>(dirLoop);
                                if (maze.getPassage(pos, dir)) {
                                    _debugText += FString::Printf(TEXT("passage Position(%i,%i,%i,%i,%i,%i) direction %s ==== "), uLoop, vLoop, wLoop, zLoop, yLoop, xLoop, DIRECTION_TEXT[dirLoop]);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    _debugText += FString::Printf(TEXT("\n=======================================\n"));
    _mazeBuild->startMaze();
    _win_showing = false;
    _win_shown = false;
    _playerRotator = INIT_PLAYER_ROTATION;
    _playerForward = INIT_PLAYER_FORWARD;
    MovePlayerTo(START);
    _playerController->SetControlRotation(INIT_PLAYER_ROTATION);
}

/**
 * new UE4 map, pass map data as parms
 *
 * @param {AMazeBuild*} mazeBuild
 * @param {APlayerController*} playerController
 * @param {ASkyLight*} skyLight
 */
void UTouchBlueprintFunctionLibrary::newMap(AMazeBuild* mazeBuild, APlayerController* playerController, ASkyLight* skyLight) {
    _mazeBuild = mazeBuild;
    APawn* pawn = playerController->GetPawn();
    ACharacter* player = static_cast<ACharacter*>(pawn);
    _skyLightComponent = skyLight->GetLightComponent();
    setPlayer(player);
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
    _playerMoving = false;
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
 * Rotates the player in the specified rotation.
 *
 * @param {FRotator} rotator
 */
void UTouchBlueprintFunctionLibrary::rotatePlayer(FRotator rotator) {
    FRotator currentRotation = _playerController->GetControlRotation();
    FRotator newRotation = currentRotation.Add(rotator.Pitch, rotator.Yaw, rotator.Roll);
    _playerController->SetControlRotation(newRotation);
    FVector tempForward = FRotationMatrix(newRotation).GetScaledAxis(EAxis::X);
    _playerForward = FVector((float)round(tempForward.X), (float)round(tempForward.Y), (float)round(tempForward.Z));
    _playerRotator = FRotator((float)(((int)round(newRotation.Pitch)) % 360), (float)(((int)round(newRotation.Yaw)) % 360), (float)(((int)round(newRotation.Roll)) % 360));
    Direction dir = U_PLUS;
    if (_playerForward.X == +1) {
        dir = EAST;
    } else if (_playerForward.X == -1) {
        dir = WEST;
    } else if (_playerForward.Y == +1) {
        dir = SOUTH;
    } else if (_playerForward.Y == -1) {
        dir = NORTH;
    } else if (_playerForward.Z == +1) {
        dir = UP_;
    } else if (_playerForward.Z == -1) {
        dir = DOWN_;
    }
    _mazeBuild->ChangeDirection(dir);
}

/**
 * user presses/unpresses up button
 *
 * @param {bool} isPress
 */
void UTouchBlueprintFunctionLibrary::turnPlayerUp(bool isPress) {
    if (isPress && _playerForward != UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[UP_]) {
        _debugText += FString::Printf(TEXT(" UP ==== "));
        rotatePlayer(FRotator(+90, 0, 0));
    }
}

/**
 * user presses/unpresses down button
 *
 * @param {bool} isPress
 */
void UTouchBlueprintFunctionLibrary::turnPlayerDown(bool isPress) {
    if (isPress && _playerForward != UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[DOWN_]) {
        _debugText += FString::Printf(TEXT(" DOWN ==== "));
        rotatePlayer(FRotator(-90, 0, 0));
    }
}

/**
 * user presses/unpresses left button
 *
 * @param {bool} isPress
 */
void UTouchBlueprintFunctionLibrary::turnPlayerLeft(bool isPress) {
    if (isPress) {
        _debugText += FString::Printf(TEXT(" LEFT ==== "));
        rotatePlayer(FRotator(0, -90, 0));
   }
}

/**
 * user presses/unpresses right button
 *
 * @param {bool} isPress
 */
void UTouchBlueprintFunctionLibrary::turnPlayerRight(bool isPress) {
    if (isPress) {
        _debugText += FString::Printf(TEXT(" RIGHT ==== "));
        rotatePlayer(FRotator(0, +90, 0));
    }
}

/**
 * user presses/unpresses reverse button
 *
 * @param {bool} isPress
 */
void UTouchBlueprintFunctionLibrary::turnPlayerReverse(bool isPress) {
    if (isPress) {
        _debugText += FString::Printf(TEXT(" REVERSE ==== "));
        if (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[UP_]) {
            rotatePlayer(FRotator(-180, 0, 0));
        } else if (_playerForward == UTouchBlueprintFunctionLibrary::PLAYER_FORWARD[DOWN_]) {
            rotatePlayer(FRotator(+180, 0, 0));
        } else {
            rotatePlayer(FRotator(0, +180, 0));
        }
    }
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

/**
 * update user settings, dimension size in each dimension
 *
 * @param {int} uSize
 * @param {int} vSize
 * @param {int} wSize
 * @param {int} xSize
 * @param {int} ySize
 * @param {int} zSize
 */
void UTouchBlueprintFunctionLibrary::updateSettings(int uSize, int vSize, int wSize, int xSize, int ySize, int zSize) {
    _u_size = uSize;
    _v_size = vSize;
    _w_size = wSize;
    _width  = xSize;
    _height = ySize;
    _depth  = zSize;
    initMaze();
}

/**
 * change walls to new wall selecion
 *
 */
void UTouchBlueprintFunctionLibrary::changeWall() {
    _mazeBuild->changeWall();
}

/**
 * get the tet to sidplay in the users debug screen
 *
 * @return {FString}
 */
FString UTouchBlueprintFunctionLibrary::getDebugText() {
    return _debugText;
}

/**
 * start or stop moving a player forward if the key is pressed or not
 *
 * @param {bool} isPress
 */
void UTouchBlueprintFunctionLibrary::movePlayer(bool isPress) {
    _playerMoving = isPress;
}

/**
 * Display that the player has won, reached the end Call.
 *
 */
void UTouchBlueprintFunctionLibrary::ShowWin() {
    _playerMoving = false;
    _win_showing = true;
    _win_showing_time = 0.0;
    _win_shown = true;
    _mazeBuild->playWin();
}

// Called every frame
void UTouchBlueprintFunctionLibrary::Tick(float DeltaTime)
{
    if (_playerMoving) {
        Position savePlayerPosition = _playerPosition;
        _player->AddMovementInput(_playerForward, _playerSpeed, true);
        _playerPosition = _mazeBuild->locationToPosition(_playerPosition.getU(), _playerPosition.getV(), _playerPosition.getW(), _player->GetActorLocation());
        if (!(_playerPosition == savePlayerPosition)) {
            _debugText += FString::Printf(TEXT(" MOVETO (%i,%i,%i,%i,%i,%i) ==== "), _playerPosition.getU(), _playerPosition.getV(), _playerPosition.getW(), _playerPosition.getZ(), _playerPosition.getY(), _playerPosition.getX());
        }
        if (_playerPosition == _mazeBuild->GetMaze().getEnd() && !_win_shown) {
            ShowWin();
        }
    }
    if (_win_showing) {
        _win_showing_time += DeltaTime;
        if (_win_showing_time > WIN_DISPLAY_TIME) {
            _win_showing = false;
            _mazeBuild->setWin();
            _skyLightComponent->SetLightColor(FLinearColor(1.0, 1.0, 1.0, 1.0));
        }
        else {
            int win_show_percent = (int)(100.0 * _win_showing_time / WIN_DISPLAY_TIME);
            int display_percent = 6 * win_show_percent;
            float InR = (display_percent % 100) / 100.0;
            float InG = ((display_percent + 33) % 100) / 100.0;
            float InB = ((display_percent + 67) % 100) / 100.0;
            float InA = 1.0;
            _skyLightComponent->SetLightColor(FLinearColor(InR, InG, InB, InA));
        }
    }
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
