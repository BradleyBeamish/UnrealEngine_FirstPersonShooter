#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ShooterGameStateBase.generated.h"

UCLASS()
class BBSHOOTER_API AShooterGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	int CurrentEnemyCount = 0;

	int CurrentWave = 1;

	int MaxEnemyCount = 22;

	int MaxPlayerHealth = 100;

	int PlayerHealth;

	void EnemyDestroyed();
	
	void StartNextWave();

	void RestartGame();

	void SetPlayerHealth(int Health);
};
