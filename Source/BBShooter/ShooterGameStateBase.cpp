#include "ShooterGameStateBase.h"

#include "EngineUtils.h"
#include "ShooterCharacter.h"
#include "ShooterHUD.h"
#include "ShooterMovingEnemySpawner.h"
#include "ShooterTower.h"
#include "Kismet/GameplayStatics.h"

/*
 * Called when a Moving Enemy or Tower is destroyed
 * When 0 enemies are left, the next wave will start.
 */
void AShooterGameStateBase::EnemyDestroyed()
{
	CurrentEnemyCount--;

	AShooterHUD* HUD = Cast<AShooterHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (HUD)
	{
		float Percentage = static_cast<float>(CurrentEnemyCount) / MaxEnemyCount;
		HUD->UpdateEnemyPercentage(Percentage);
	}

	UE_LOG(LogTemp, Warning, TEXT("Current Enemy Count: %d"), CurrentEnemyCount);

	if (CurrentEnemyCount <= 0)
	{
		// 5 Seconds Between Rounds
		FTimerHandle WaveDelayTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(WaveDelayTimerHandle, this, &AShooterGameStateBase::StartNextWave, 5.0f, false);
	}
}

/*
 * Starts the next wave.
 * Finds all the towers and spawners and sets them up to work again
 */
void AShooterGameStateBase::StartNextWave()
{
	CurrentWave++;

	AShooterHUD* HUD = Cast<AShooterHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (HUD)
	{
		if (CurrentWave == 6)
		{
			HUD->UpdateWinStatus(true); // Player has beaten 5/5 waves, they've won
		} else
		{
			HUD->UpdateWaveNumber(CurrentWave); // Continue on with next wave
		}
	}

	// Find all Moving Enemy Spawners in world, set them up for a new wave
	TArray<AActor*> FoundSpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterMovingEnemySpawner::StaticClass(), FoundSpawners);
	
	for (AActor* Actor : FoundSpawners)
	{
		AShooterMovingEnemySpawner* Spawner = Cast<AShooterMovingEnemySpawner>(Actor);
		if (Spawner)
		{
			Spawner->NextWave();
		}
	}

	// Re-enable all towers (currently in a disable state)
	for (TActorIterator<AShooterTower> It(GetWorld()); It; ++It)
	{
		AShooterTower* Tower = *It;
		if (Tower)
		{
			Tower->EnableTower();
		}
	}
}

/*
 * Called when "R" Key is pressed
 * Removes all enemies, Disables towers, Moves player to starting position
 */
void AShooterGameStateBase::RestartGame()
{
	// Remove all moving enemies from world
	for (TActorIterator<AShooterMovingEnemy> It(GetWorld()); It; ++It)
	{
		AShooterMovingEnemy* Enemy = *It;
		if (Enemy)
		{
			Enemy->Destroy();
		}
	}

	// Disables all Towers
	for (TActorIterator<AShooterTower> It(GetWorld()); It; ++It)
	{
		AShooterTower* Tower = *It;
		if (Tower)
		{
			Tower->DisableTower();
		}
	}

	CurrentWave = 0;
	CurrentEnemyCount = 0;

	// Update the HUD to reflect new game status
	AShooterHUD* HUD = Cast<AShooterHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (HUD)
	{
		HUD->UpdateWaveNumber(CurrentWave);
		HUD->UpdateEnemyPercentage(1.0f);
		HUD->UpdateWinStatus(false);
	}

	// Reset Character
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AShooterCharacter::StaticClass()));
	
	if (ShooterCharacter)
	{
		ShooterCharacter->ResetCharacter();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Restarting Game"));

	StartNextWave();
}

/*
 * Middleman between AShooterCharacter and AHUD
 */
void AShooterGameStateBase::SetPlayerHealth(int Health)
{
	PlayerHealth = Health;

	AShooterHUD* HUD = Cast<AShooterHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (HUD)
	{
		float Percentage = static_cast<float>(PlayerHealth) / MaxPlayerHealth;
		HUD->UpdatePlayerHealth(Percentage);

		if (PlayerHealth <= 0)
		{
			HUD->UpdateGameState(true);
		} else
		{
			HUD->UpdateGameState(false);
		}
	}
}
