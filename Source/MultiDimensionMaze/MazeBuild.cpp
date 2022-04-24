#include "MazeBuild.h"

/* constants */
const FVector AMazeBuild::DELTA_WALL[DIRECTION_COUNT] = {
	FVector::ZeroVector,FVector::ZeroVector,FVector::ZeroVector,FVector::ZeroVector,FVector::ZeroVector,FVector::ZeroVector,
	FVector(-0.5,  0.0, +0.5),
	FVector(-0.5,  0.0, -0.5),
	FVector( 0.0, +0.5, -0.5),
	FVector( 0.0, -0.5, -0.5),
	FVector( 0.5,  0.0, -0.5),
	FVector(-0.5,  0.0, -0.5)
};

const FVector AMazeBuild::FIREWORKS_DELTA_END_LOCATION[9] = {
	FVector(0, 0, 0),
	FVector(-FIREWORKS_DELTA_DELTA_END_LOCATION, -FIREWORKS_DELTA_DELTA_END_LOCATION, -FIREWORKS_DELTA_DELTA_END_LOCATION),
	FVector(-FIREWORKS_DELTA_DELTA_END_LOCATION, -FIREWORKS_DELTA_DELTA_END_LOCATION, +FIREWORKS_DELTA_DELTA_END_LOCATION),
	FVector(-FIREWORKS_DELTA_DELTA_END_LOCATION, +FIREWORKS_DELTA_DELTA_END_LOCATION, -FIREWORKS_DELTA_DELTA_END_LOCATION),
	FVector(-FIREWORKS_DELTA_DELTA_END_LOCATION, +FIREWORKS_DELTA_DELTA_END_LOCATION, +FIREWORKS_DELTA_DELTA_END_LOCATION),
	FVector(+FIREWORKS_DELTA_DELTA_END_LOCATION, -FIREWORKS_DELTA_DELTA_END_LOCATION, -FIREWORKS_DELTA_DELTA_END_LOCATION),
	FVector(+FIREWORKS_DELTA_DELTA_END_LOCATION, -FIREWORKS_DELTA_DELTA_END_LOCATION, +FIREWORKS_DELTA_DELTA_END_LOCATION),
	FVector(+FIREWORKS_DELTA_DELTA_END_LOCATION, +FIREWORKS_DELTA_DELTA_END_LOCATION, -FIREWORKS_DELTA_DELTA_END_LOCATION),
	FVector(+FIREWORKS_DELTA_DELTA_END_LOCATION, +FIREWORKS_DELTA_DELTA_END_LOCATION, +FIREWORKS_DELTA_DELTA_END_LOCATION),
};

static const FVector WALL_SCALE(AMazeBuild::WALL_THICKNESS, AMazeBuild::WALL_SIZE, AMazeBuild::WALL_SIZE);
static const FVector TERMINATION_SCALE(AMazeBuild::TERMINATION_DIAMETER, AMazeBuild::TERMINATION_DIAMETER, AMazeBuild::TERMINATION_DIAMETER);
static const FVector LETTER_SCALE(TEXT_SCALE, TEXT_SCALE, TEXT_SCALE);

/**
 * AMazeBuild is the Actor in place to build the visual Maze 
 *
 * @constructor
 */
AMazeBuild::AMazeBuild() {
	ConstructorHelpers::FObjectFinder<UStaticMesh>WallMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Cube.Shape_Cube'"));
	WallMeshRef = WallMesh.Object;
	ConstructorHelpers::FObjectFinder<UStaticMesh>PlusMesh(TEXT("StaticMesh'/Game/Shapes/plus.plus'"));
	PlusMeshRef = PlusMesh.Object;
	ConstructorHelpers::FObjectFinder<UStaticMesh>MinusMesh(TEXT("StaticMesh'/Game/Shapes/minus.minus'"));
	MinusMeshRef = MinusMesh.Object;
	ConstructorHelpers::FObjectFinder<UStaticMesh>StartMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Cube.Shape_Cube'"));
	StartMeshRef = StartMesh.Object;
	ConstructorHelpers::FObjectFinder<UStaticMesh>EndMesh(TEXT("StaticMesh'/Game/Shapes/Shape_Sphere.Shape_Sphere'"));
	EndMeshRef = EndMesh.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial> StartMaterialRef(TEXT("Material'/Game/Materials/Start.Start'"));
	StartMaterial = StartMaterialRef.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial> EndMaterialRef(TEXT("Material'/Game/Materials/End.End'"));
	EndMaterial = EndMaterialRef.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> WallParamRef(TEXT("Material'/Game/Materials/Wall_Param.Wall_Param'"));
	WallParam = WallParamRef.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> UDimensionMaterialRef(TEXT("Material'/Game/Materials/UDimension.UDimension'"));
	UDimensionMaterial = UDimensionMaterialRef.Object;
	DIMENSION_MATERIAL.push_back(UDimensionMaterial);
	DIMENSION_MATERIAL.push_back(UDimensionMaterial);
	static ConstructorHelpers::FObjectFinder<UMaterial> VDimensionMaterialRef(TEXT("Material'/Game/Materials/VDimension.VDimension'"));
	VDimensionMaterial = VDimensionMaterialRef.Object;
	DIMENSION_MATERIAL.push_back(VDimensionMaterial);
	DIMENSION_MATERIAL.push_back(VDimensionMaterial);
	static ConstructorHelpers::FObjectFinder<UMaterial> WDimensionMaterialRef(TEXT("Material'/Game/Materials/WDimension.WDimension'"));
	WDimensionMaterial = WDimensionMaterialRef.Object;
	DIMENSION_MATERIAL.push_back(WDimensionMaterial);
	DIMENSION_MATERIAL.push_back(WDimensionMaterial);
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// Init Audio Resource
//	static ConstructorHelpers::FObjectFinder<USoundWave> fireworkAudioResource(TEXT("SoundWave'/Game/Sounds/336008__rudmer-rotteveel__whistle-and-explosion-single-firework-cropped.336008__rudmer-rotteveel__whistle-and-explosion-single-firework-cropped'"));
//	FireworkSoundWave = fireworkAudioResource.Object;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> fireworks(TEXT("ParticleSystem '/Game/Materials/Fireworks_P.Fireworks_P'"));
	FireworksParticleSystem = fireworks.Object;

	FString PathToLoad;
	// set Textures
	PathToLoad = "Texture2D'/Game/Textures/T_Brick_Clay_Old_D.T_Brick_Clay_Old_D'";
	TextureColor[0] = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *PathToLoad));
	PathToLoad = "Texture2D'/Game/Textures/T_Brick_Clay_Old_N.T_Brick_Clay_Old_N'";
	TextureNormal[0] = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *PathToLoad));
	PathToLoad = "Texture2D'/Game/Textures/T_Rock_Basalt_D.T_Rock_Basalt_D'";
	TextureColor[1] = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *PathToLoad));
	PathToLoad = "Texture2D'/Game/Textures/T_Rock_Basalt_N.T_Rock_Basalt_N'";
	TextureNormal[1] = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *PathToLoad));
	PathToLoad = "Texture2D'/Game/Textures/T_Metal_Rust_D.T_Metal_Rust_D'";
	TextureColor[2] = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *PathToLoad));
	PathToLoad = "Texture2D'/Game/Textures/T_Metal_Rust_N.T_Metal_Rust_N'";
	TextureNormal[2] = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *PathToLoad));
	PathToLoad = "Texture2D'/Game/Textures/T_Wood_Pine_D.T_Wood_Pine_D'";
	TextureColor[3] = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *PathToLoad));
	PathToLoad = "Texture2D'/Game/Textures/T_Wood_Pine_N.T_Wood_Pine_N'";
	TextureNormal[3] = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *PathToLoad));
	PathToLoad = "Texture2D'/Game/Textures/T_Metal_Steel_D.T_Metal_Steel_D'";
	TextureColor[4] = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *PathToLoad));
	PathToLoad = "Texture2D'/Game/Textures/T_Metal_Steel_N.T_Metal_Steel_N'";
	TextureNormal[4] = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *PathToLoad));
}

// Called when the game starts or when spawned
void AMazeBuild::BeginPlay() {
	Super::BeginPlay();
	// set Material offsets
	UMaterialInstanceDynamic* Wall0Material = UMaterialInstanceDynamic::Create(WallParam, this, FName("Wall_0_Param"));
	WallMaterials[0] = Wall0Material;
	UMaterialInstanceDynamic* Wall1Material = UMaterialInstanceDynamic::Create(WallParam, this, FName("Wall_1_Param"));
	WallMaterials[1] = Wall1Material;
	UMaterialInstanceDynamic* Wall2Material = UMaterialInstanceDynamic::Create(WallParam, this, FName("Wall_2_Param"));
	WallMaterials[2] = Wall2Material;
	UMaterialInstanceDynamic* Wall3Material = UMaterialInstanceDynamic::Create(WallParam, this, FName("Wall_3_Param"));
	WallMaterials[3] = Wall3Material;
	UMaterialInstanceDynamic* Wall4Material = UMaterialInstanceDynamic::Create(WallParam, this, FName("Wall_4_Param"));
	WallMaterials[4] = Wall4Material;
	UMaterialInstanceDynamic* Wall5Material = UMaterialInstanceDynamic::Create(WallParam, this, FName("Wall_5_Param"));
	WallMaterials[5] = Wall5Material;
}

/**
 * Creates (replaces) a new Maze with the specified dimensions
 *
 * @param {int} u_size
 * @param {int} v_size
 * @param {int} w_size
 * @param {int} depth
 * @param {int} height
 * @param {int} width
 */
void AMazeBuild::NewMaze(int u_size, int v_size, int w_size, int depth, int height, int width) {
	_maze = Maze(u_size, v_size, w_size, depth, height, width);
	backtrackGenerator.generate(_maze);
	_maze.solve();
}

/**
 * Create a wall in the specified Direction at the specified Position
 *
 * @param {Direction} dir
 * @param {Position} pos
 * @return {UInstancedStaticMeshComponent*} new wall
 */
UInstancedStaticMeshComponent* AMazeBuild::createWall(Direction dir, Position pos) {
	char id[100];
	sprintf(id, "wall_%i_%i_%i_%i_%i_%i_%i", dir, pos.getU(), pos.getV(), pos.getW(), pos.getZ(), pos.getY(), pos.getX());
	UInstancedStaticMeshComponent* WallComp = NewObject<UInstancedStaticMeshComponent>(this, id);
	FRotator wallRotator = WALL_ROTATOR[dir];
	WallComp->RegisterComponent();
	WallComp->SetStaticMesh(WallMeshRef);
	WallComp->SetFlags(RF_Transactional);
	this->AddInstanceComponent(WallComp);
	FVector wallLocation = positionToLocation(pos, DELTA_WALL[dir]);
	FTransform wallTransform(wallRotator, wallLocation, WALL_SCALE);
	WallComp->AddInstance(wallTransform);
	WallComp->SetMaterial(0, WallMaterials[dir - UP_]);
	WallComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
 	WallComp->SetVisibility(true);
	return WallComp;
}

/**
 * Create a termination (start/end display) at the specified Position
 *
 * @param {Position} pos
 * @param {bool} isStart
 */
void AMazeBuild::Termination(Position pos, bool isStart) {
	UInstancedStaticMeshComponent* TerminationComp = NewObject<UInstancedStaticMeshComponent>(this, (pos == START) ? "START" : "END");
	FRotator terminationRotator = NO_ROTATOR;
	TerminationComp->RegisterComponent();
	TerminationComp->SetStaticMesh((pos == START) ? StartMeshRef : EndMeshRef);
	TerminationComp->SetFlags(RF_Transactional);
	this->AddInstanceComponent(TerminationComp);
	FVector terminationLocation = positionToLocation(pos);
	FTransform transform(terminationRotator, terminationLocation, TERMINATION_SCALE);
	TerminationComp->AddInstance(transform);
	TerminationComp->SetMaterial(0, (pos == START) ? StartMaterial : EndMaterial);
	TerminationComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TerminationComp->SetVisibility(true);
	if (isStart) {
		startTermination = TerminationComp;
	}
	else {
		endTermination = TerminationComp;
	}
}

/**
 * Create an exterior wall at the specified Position. This wall is
 * created seperately because it does not have a Maze Position
 *
 * @param {Position} pos
 */
void AMazeBuild::OuterWall(Position pos) {
	if (pos.getZ() == 0) {
		UInstancedStaticMeshComponent* wall = createWall(DOWN_, pos);
		walls[pos.getZ()][pos.getY()][pos.getX()][DOWN_] = wall;
	}
	if (pos.getY() == 0) {
		UInstancedStaticMeshComponent* wall = createWall(NORTH, pos);
		walls[pos.getZ()][pos.getY()][pos.getX()][NORTH] = wall;
	}
	if (pos.getX() == 0) {
		UInstancedStaticMeshComponent* wall = createWall(WEST, pos);
		walls[pos.getZ()][pos.getY()][pos.getX()][WEST] = wall;
	}
}

/**
 * Create a wall at the specified Position. This wall does have a
 * Maze Position
 *
 * @param {Position} pos
 */
void AMazeBuild::InnerWall(Position pos) {
	if (pos.getZ() < GetMaze().getDepth()) {
		UInstancedStaticMeshComponent* wall = createWall(UP_, pos);
		walls[pos.getZ()][pos.getY()][pos.getX()][UP_] = wall;
	}
	if (pos.getY() < GetMaze().getHeight()) {
		UInstancedStaticMeshComponent* wall = createWall(SOUTH, pos);
		walls[pos.getZ()][pos.getY()][pos.getX()][SOUTH] = wall;
	}
	if (pos.getX() < GetMaze().getWidth()) {
		UInstancedStaticMeshComponent* wall = createWall(EAST, pos);
		walls[pos.getZ()][pos.getY()][pos.getX()][EAST] = wall;
	}
}

/**
 * Rotate the set of dimension letters in the specified Direction.
 *
 * @param {USceneComponent*} letters
 * @param {Direction} newDir
 */
void AMazeBuild::RotateDimensionLetters(USceneComponent* letters, Direction newDir) {
	FRotator rotator = DIM_LETTER_ROTATORS[newDir];
	letters->SetRelativeRotation(rotator);
}

/**
 * Create a dimension letter at the specified Position in the specified Direction.
 *
 * @param {Position} pos
 * @param {Direction} dir
 * @return {UInstancedStaticMeshComponent*} dimension letter
 */
UInstancedStaticMeshComponent* AMazeBuild::DimensionLetter(Position pos, Direction dir) {
	char id[100];
	sprintf(id, "dim_letter_%i_%i_%i_%i_%i_%i_%i", pos.getU(), pos.getV(), pos.getW(), pos.getZ(), pos.getY(), pos.getX(), dir);
	UInstancedStaticMeshComponent* letter = NewObject<UInstancedStaticMeshComponent>(this, id);
	letter->RegisterComponent();
	letter->SetStaticMesh((dir % 2)  == 0 ? PlusMeshRef : MinusMeshRef);
	letter->SetFlags(RF_Transactional);
	this->AddInstanceComponent(letter);
	double letterX = CUBE_ACTUAL_SIZE * (0);
	double letterY = CUBE_ACTUAL_SIZE * (DIMENSION_TEXT_HORZ[dir]);
	double letterZ = CUBE_ACTUAL_SIZE * (DIMENSION_TEXT_VERT[dir]);
	FVector relativeLocation(letterX, letterY, letterZ);
	FVector scale(DIM_LETTER_LONG, DIM_LETTER_SHORT, DIM_LETTER_SHORT);
	FTransform transform(NO_ROTATOR, relativeLocation, FVector::OneVector);
	letter->AddInstance(transform);
	UMaterial* material = DIMENSION_MATERIAL[dir];
	letter->SetMaterial(0, material);
	letter->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	letter->SetVisibility(true);
	return letter;
}

/**
 * Create a dimension letter at the specified Position.
 *
 * @param {Position} pos
 */
void AMazeBuild::DimensionText(Position pos) {
	FVector lettersLocation = positionToLocation(pos, FVector::ZeroVector);
	char id[100];
	sprintf(id, "dim_letters_%i_%i_%i_%i_%i_%i", pos.getU(), pos.getV(), pos.getW(), pos.getZ(), pos.getY(), pos.getX());
	USceneComponent* letters = NewObject <USceneComponent>(this, id, RF_NoFlags, nullptr, false, nullptr);
	letters->SetRelativeLocation(lettersLocation);
	for (int dirLoop = U_PLUS; dirLoop <= W_MINUS; dirLoop++) {
		Direction dir = static_cast<Direction>(dirLoop);
		UInstancedStaticMeshComponent* letter = DimensionLetter(pos, dir);
		dimensionLetter[pos.getZ()][pos.getY()][pos.getX()][dir] = letter;
		letter->AttachToComponent(letters, FAttachmentTransformRules::KeepRelativeTransform);
	}
	dimensionLetters[pos.getZ()][pos.getY()][pos.getX()] = letters;
}

/**
 * Display all the parts for the Cell at the specified Position.
 *
 * @param {Position} pos
 */
void AMazeBuild::ProcessCell(Position pos) {
	OuterWall(pos);
	InnerWall(pos);
	DimensionText(pos);
}

/**
 * Clear the Maze Build
 *
 */
void AMazeBuild::clearBuild() {
	TArray<UActorComponent*> components = this->GetInstanceComponents();
	for (UActorComponent* component : components) {
		this->RemoveInstanceComponent(component);
		component->DestroyComponent();
	}
	walls.clear();
	dimensionLetter.clear();
	dimensionLetters.clear();
}

/**
 * Display the new Maze.
 *
 */
void AMazeBuild::DisplayMaze() {
	clearBuild();
	walls.resize(GetMaze().getDepth()+1);
	dimensionLetters.resize(GetMaze().getDepth() + 1);
	dimensionLetter.resize(GetMaze().getDepth() + 1);
	for (int z_loop = 0; z_loop < GetMaze().getDepth() + 1; z_loop++) {
		walls[z_loop].resize(GetMaze().getHeight() + 1);
		dimensionLetters[z_loop].resize(GetMaze().getHeight() + 1);
		dimensionLetter[z_loop].resize(GetMaze().getHeight() + 1);
		for (int y_loop = 0; y_loop < GetMaze().getHeight() + 1; y_loop++) {
			walls[z_loop][y_loop].resize(GetMaze().getWidth() + 1);
			dimensionLetters[z_loop][y_loop].resize(GetMaze().getWidth() + 1);
			dimensionLetter[z_loop][y_loop].resize(GetMaze().getWidth() + 1);
			for (int x_loop = 0; x_loop < GetMaze().getWidth() + 1; x_loop++) {
				walls[z_loop][y_loop][x_loop].resize(DIRECTION_COUNT);
				dimensionLetter[z_loop][y_loop][x_loop].resize(DIRECTION_COUNT);
			}
		}
	}

	int uLoop = selectU;
	int vLoop = selectV;
	int wLoop = selectW;
//	for (int uLoop = 0; uLoop < _maze->getUSize(); uLoop++) {
//		for (int vLoop = 0; vLoop < _maze->getVSize(); vLoop++) {
//			for (int wLoop = 0; wLoop < _maze->getWSize(); wLoop++) {
				for (int zLoop = 0; zLoop <= GetMaze().getDepth(); zLoop++) {
					for (int yLoop = 0; yLoop <= GetMaze().getHeight(); yLoop++) {
						for (int xLoop = 0; xLoop <= GetMaze().getWidth(); xLoop++) {
							ProcessCell(Position(uLoop, vLoop, wLoop, zLoop, yLoop, xLoop));
						}
					}
				}
//			}
//		}
//	}
	Termination(START, true);
	Termination(Position(0, 0, 0, GetMaze().getDepth() - 1, GetMaze().getHeight() - 1, GetMaze().getWidth() - 1), false);
}

/**
 * Change the Direction of the player.
 *
 * @param {Direction} newDir
 */
void AMazeBuild::ChangeDirection(Direction newDir) {
	for (int zLoop = 0; zLoop <= GetMaze().getDepth(); zLoop++) {
		for (int yLoop = 0; yLoop <= GetMaze().getHeight(); yLoop++) {
			for (int xLoop = 0; xLoop <= GetMaze().getWidth(); xLoop++) {
				Position pos(selectU, selectV, selectW, zLoop, yLoop, xLoop);
				USceneComponent* letters = dimensionLetters[pos.getZ()][pos.getY()][pos.getX()];
				RotateDimensionLetters(letters, newDir);
			}
		}
	}
}

/**
 * Update the Maze display for a changed dimension for movement at the specified Position in the specified Direction.
 *
 * @param {Position} pos
 * @param {Direction} dir
 * @return {Position} new Position
 */
Position AMazeBuild::MoveDimension(Position pos, Direction dir) {
	Position newPosition = pos.neighbor(dir);
	SetDimension(newPosition);
	return newPosition;
}

/**
 * Update the Maze display for a changed dimension for the specified Position.
 *
 * @param {Position} pos
 */
void AMazeBuild::SetDimension(Position pos) {
	selectU = pos.getU();
	selectV = pos.getV();
	selectW = pos.getW();
	for (int zLoop = 0; zLoop <= GetMaze().getDepth(); zLoop++) {
		for (int yLoop = 0; yLoop <= GetMaze().getHeight(); yLoop++) {
			for (int xLoop = 0; xLoop <= GetMaze().getWidth(); xLoop++) {
				for (Direction dirLoop : wallDirs) {
					UInstancedStaticMeshComponent* wall = walls[zLoop][yLoop][xLoop][dirLoop];
					if (wall != NULL) {
						if (zLoop < GetMaze().getDepth() && yLoop < GetMaze().getHeight() && xLoop < GetMaze().getWidth() && GetMaze().getPassage(Position(selectU, selectV, selectW, zLoop, yLoop, xLoop), dirLoop)) {
							wall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
							wall->SetVisibility(false, true);
						}
						else {
							wall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
							wall->SetVisibility(true, true);
						}
					}
				}
				for (int dirLoop = MIN_VIRTUAL_DIRECTION; dirLoop < MAX_VIRTUAL_DIRECTION + 1; dirLoop++) {
					Direction dir = static_cast<Direction>(dirLoop);
					UInstancedStaticMeshComponent* letter = dimensionLetter[zLoop][yLoop][xLoop][dir];
					if (zLoop < GetMaze().getDepth() && yLoop < GetMaze().getHeight() && xLoop < GetMaze().getWidth() && GetMaze().getPassage(Position(selectU, selectV, selectW, zLoop, yLoop, xLoop), dir)) {
						letter->SetVisibility(true);
					}
					else {
						letter->SetVisibility(false);
					}
				}
			}
		}
	}
	startTermination->SetVisibility(pos.getU() == 0 && pos.getV() == 0 && pos.getW() == 0);
	endTermination->SetVisibility(pos.getU() == GetMaze().getEnd().getU() && pos.getV() == GetMaze().getEnd().getV() && pos.getW() == GetMaze().getEnd().getW());

	int dimOffset = pos.getU() + pos.getV() + pos.getW();
	WallMaterials[0]->SetScalarParameterValue("U_offset", (dimOffset +  0) % 3);
	WallMaterials[0]->SetScalarParameterValue("V_offset", (dimOffset +  0) % 2);
	WallMaterials[1]->SetScalarParameterValue("U_offset", (dimOffset +  1) % 3);
	WallMaterials[1]->SetScalarParameterValue("V_offset", (dimOffset +  0) % 2);
	WallMaterials[2]->SetScalarParameterValue("U_offset", (dimOffset +  2) % 3);
	WallMaterials[2]->SetScalarParameterValue("V_offset", (dimOffset +  0) % 2);
	WallMaterials[3]->SetScalarParameterValue("U_offset", (dimOffset +  0) % 3);
	WallMaterials[3]->SetScalarParameterValue("V_offset", (dimOffset +  1) % 2);
	WallMaterials[4]->SetScalarParameterValue("U_offset", (dimOffset +  1) % 3);
	WallMaterials[4]->SetScalarParameterValue("V_offset", (dimOffset +  1) % 2);
	WallMaterials[5]->SetScalarParameterValue("U_offset", (dimOffset +  2) % 3);
	WallMaterials[5]->SetScalarParameterValue("V_offset", (dimOffset +  1) % 2);
}

/**
 * Construct the new Maze.
 *
 */
void AMazeBuild::startMaze() {
	DisplayMaze();
	SetDimension(START);
	ChangeDirection(EAST);
}

/**
 * Convert a Position and delta vector to a location (vector for x,y,z)
 *
 * @param {Position} pos
 * @param {FVector} delta
 * @return {FVector} vector for x,y,z
 */
FVector AMazeBuild::positionToLocation(Position pos, FVector delta) {
	return FVector(-((float)pos.getX() + 0.5 + delta.X) * AMazeBuild::CUBE_ACTUAL_SIZE, ((float)pos.getY() + 0.5 + delta.Y) * AMazeBuild::CUBE_ACTUAL_SIZE, ((float)pos.getZ() + 0.5 + delta.Z) * AMazeBuild::CUBE_ACTUAL_SIZE);
}

/**
 * Convert a Position to a location (vector for x,y,z)
 *
 * @param {Position} pos
 * @return {FVector} vector for x,y,z
 */
FVector AMazeBuild::positionToLocation(Position pos) {
	return positionToLocation(pos, FVector::ZeroVector);
}

/**
 * Convert a location (u,v,w and Vector for x,y,z) into a Position.
 *
 * @param {int} u
 * @param {int} v
 * @param {int} w
 * @param {FVector} location
 * @return {Position*} position
 */
Position AMazeBuild::locationToPosition(int u, int v, int w, FVector location) {
	return Position(u, v, w, round((location.Z / AMazeBuild::CUBE_ACTUAL_SIZE) - 0.5), round(location.Y / AMazeBuild::CUBE_ACTUAL_SIZE - 0.5), round(-location.X / AMazeBuild::CUBE_ACTUAL_SIZE - 0.5));
}

/**
 * Get the current Maze.
 *
 * @return {Maze*} maze
 */
Maze AMazeBuild::GetMaze() {
	return _maze;
}

/**
 * Change Wall texture for all walls to the newly selected material.
 *
 */
void AMazeBuild::changeWall() {
	WallTextureIndex = (WallTextureIndex + 1) % TEXTURE_COUNT;
	for (int dirLoop = 0; dirLoop <= WALL_TEXTURE_COUNT; dirLoop++) {
		WallMaterials[dirLoop]->SetTextureParameterValue(FName("color"), TextureColor[WallTextureIndex]);
		WallMaterials[dirLoop]->SetTextureParameterValue(FName("normal"), TextureNormal[WallTextureIndex]);
	}
}

/**
 * Show fireworks for win
 *
 */
void AMazeBuild::TriggerFireworks() {
	FVector endLocation = positionToLocation(GetMaze().getEnd());
	UGameplayStatics::SpawnEmitterAtLocation(this, FireworksParticleSystem, endLocation + FIREWORKS_DELTA_END_LOCATION[fireworksCount]);
//	UGameplayStatics::PlaySound2D(this, FireworkSoundWave);
	fireworksCount--;
	playFireworksSound();
	if (fireworksCount > 0) {
		GetWorld()->GetTimerManager().SetTimer(TriggerFireworkTimerHandle, this, &AMazeBuild::TriggerFireworks, FIREWORKS_DELTA_TIME, false);
	}
}

/**
 * Play the sound indicating that the player has won, reached the end Call.
 *
 */
void AMazeBuild::playWin() {
	startShowWin();
	fireworksCount = 9;
	TriggerFireworks();
}
