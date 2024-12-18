#include "ShooterMovingEnemySpawner.h"

#include "ShooterGameStateBase.h"
#include "Kismet/KismetMathLibrary.h"

/*
 * Sets default values
 */
AShooterMovingEnemySpawner::AShooterMovingEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	// BOX COMPONENT
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
}

/*
 * Called when the game starts
 */
void AShooterMovingEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	StartSpawning();
}

/*
 * Called every frame
 */
void AShooterMovingEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
 * When called Spawn an Enemy within the box every {SpawnDelay} seconds
 */
void AShooterMovingEnemySpawner::StartSpawning()
{
	CalculateNextSpawnDelay();

	// Start timer
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AShooterMovingEnemySpawner::SpawnEnemy, SpawnDelay, false);
}

/*
 * Spawns a single Moving Enemy
 * Will only spawn an enemy when there are less than 10
 * The Enemy is spawned in a random location within the box
 */
void AShooterMovingEnemySpawner::SpawnEnemy()
{
	if (SpawnerEnemiesCount >= MaxSpawnerEnemiesCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle); // Stop timer when max enemies hit
		return;
	}

	if (MovingEnemy)
	{
		// Get Random Location
		FVector SpawnLocation = GetRandomPointInVolume();
		FRotator SpawnRotation = FRotator::ZeroRotator;
		
		AShooterMovingEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AShooterMovingEnemy>(MovingEnemy, SpawnLocation, SpawnRotation); // Spawn the enemy

		if (SpawnedEnemy)
		{
			SpawnerEnemiesCount++; // Used locally
			
			AShooterGameStateBase* GameState = GetWorld()->GetGameState<AShooterGameStateBase>();
			if (GameState)
			{
				GameState->CurrentEnemyCount++; // Used Globally
			}
		}
	}

	CalculateNextSpawnDelay();

	// Reset the timer for the next spawn
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AShooterMovingEnemySpawner::SpawnEnemy, SpawnDelay, false);
}

/*
 * Calculates the delay between spawns (ensures things get harder over time)
 */
void AShooterMovingEnemySpawner::CalculateNextSpawnDelay()
{
	SpawnDelayRangeLow /= Difficulty;
	SpawnDelayRangeHigh /= Difficulty;

	SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
}

/*
 * Returns a random location within the UBoxComponent
 */
FVector AShooterMovingEnemySpawner::GetRandomPointInVolume() const
{
	FVector SpawnOrigin = BoxComponent->Bounds.Origin;
	FVector SpawnExtent = BoxComponent->Bounds.BoxExtent;
	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

/*
 * Will be called in the GameState class to start the next wave
 */
void AShooterMovingEnemySpawner::NextWave()
{
	if (SpawnerEnemiesCount >= MaxSpawnerEnemiesCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning new wave."));
	
		SpawnerEnemiesCount = 0;
		SpawnDelayRangeLow = 1.0f;
		SpawnDelayRangeHigh = 10.0f;
	
		StartSpawning();
	}
}

/*
 * Resets the Spawner (When "R" is pressed)
 */
void AShooterMovingEnemySpawner::ResetSpawner()
{
	SpawnerEnemiesCount = 0;
	SpawnDelayRangeLow = 1.0f;
	SpawnDelayRangeHigh = 10.0f;
	
	StartSpawning();
}
