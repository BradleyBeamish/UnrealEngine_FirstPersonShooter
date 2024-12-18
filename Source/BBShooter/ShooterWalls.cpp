#include "ShooterWalls.h"

/*
 * Sets default values
 */
AShooterWalls::AShooterWalls()
{
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	RootComponent = WallMesh;
}

/*
 * Called when the game starts
 */
void AShooterWalls::BeginPlay()
{
	Super::BeginPlay();
}

/*
 * Called every frame
 */
void AShooterWalls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
