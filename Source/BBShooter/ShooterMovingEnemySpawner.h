#pragma once

#include "CoreMinimal.h"
#include "ShooterMovingEnemy.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ShooterMovingEnemySpawner.generated.h"

UCLASS()
class BBSHOOTER_API AShooterMovingEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AShooterMovingEnemySpawner();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TSubclassOf<AShooterMovingEnemy> MovingEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float SpawnDelayRangeLow = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float SpawnDelayRangeHigh = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float Difficulty = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	int MaxSpawnerEnemiesCount = 10.0f;

	FTimerHandle SpawnTimerHandle;
	
	float SpawnDelay;

	int SpawnerEnemiesCount = 0.0f;

	void SpawnEnemy();

	void CalculateNextSpawnDelay();

	FVector GetRandomPointInVolume() const;

	void StartSpawning();

	void NextWave();

	void ResetSpawner();

protected:
	virtual void BeginPlay() override;
};
