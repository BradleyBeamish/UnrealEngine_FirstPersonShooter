#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"

UCLASS()
class BBSHOOTER_API AShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	void UpdateEnemyPercentage(float StatePercentage);

	void UpdateWaveNumber(int StateWaveNumber);

	void UpdatePlayerHealth(float StateHealthPercentage);

	void UpdateGameState(int StateGameState);

	void UpdateWinStatus(int StateYouWon);

	float HUDEnemyPercentage = 1.0f;

	float HUDPlayerHealth = 1.0f;

	int HUDWaveNumber = 1;

	int HUDGameOver = false;

	int HUDYouWon = false;
};
