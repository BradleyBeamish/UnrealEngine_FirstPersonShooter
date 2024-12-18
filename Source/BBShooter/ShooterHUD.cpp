#include "ShooterHUD.h"

/*
 * Display the HUD on Screen
 */
void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();
	
	FVector2D ScreenSize;
	GEngine->GameViewport->GetViewportSize(ScreenSize);
	int HorizontalCenterOfScreen = (ScreenSize.X) / 2.0f;
	int VerticalCenterOfScreen = (ScreenSize.Y) / 2.0f;

	// ENEMIES LEFT PERCENTAGE BAR
	DrawRect(FLinearColor::Gray, HorizontalCenterOfScreen - 700.0f, 30.0f, 700.0f, 50.0f); // Background
	DrawRect(FLinearColor::Red, HorizontalCenterOfScreen - 700.0f, 30.0f, 700.0f * HUDEnemyPercentage, 50.0f); // Foreground

	// CURRENT WAVE NUMBER TEXT
	FString WaveText = FString::Printf(TEXT("Wave Number: %d/5"), HUDWaveNumber);
	DrawText(WaveText, FLinearColor::White, 30.0f, 80.0f, nullptr, 2.5f, false);

	// PRESS R TO RESTART
	FString RestartText = FString::Printf(TEXT("Press 'R' to Restart"));
	DrawText(RestartText, FLinearColor::White, 30.0f, 120.0f, nullptr, 2.5f, false);

	// PLAYER HEALTH PERCENTAGE BAR
	DrawRect(FLinearColor::Gray, 30.0f, 30.0f, 400.0f, 50.0f); // Background
	DrawRect(FLinearColor::Green, 30.0f, 30.0f, 400.0f * HUDPlayerHealth, 50.0f); // Foreground
	FString HealthText = FString::Printf(TEXT("Health"));
	DrawText(HealthText, FLinearColor::White, 30.0f, 30.0f, nullptr, 2.5f, false);

	if (HUDGameOver)
	{
		FString GameOverText = FString::Printf(TEXT("GAME OVER"));
		DrawText(GameOverText, FLinearColor::White, HorizontalCenterOfScreen - 300.0f, VerticalCenterOfScreen, nullptr, 5.0f, false);

		FString RestartTextGameOver = FString::Printf(TEXT("Press 'R' to Restart"));
		DrawText(RestartTextGameOver, FLinearColor::White, HorizontalCenterOfScreen - 300.0f, VerticalCenterOfScreen + 70.0f, nullptr, 2.5f, false);
	}

	if (HUDYouWon)
	{
		FString YouWinText = FString::Printf(TEXT("YOU WIN!"));
		DrawText(YouWinText, FLinearColor::White, HorizontalCenterOfScreen - 300.0f, VerticalCenterOfScreen, nullptr, 5.0f, false);

		FString RestartTextYouWin = FString::Printf(TEXT("Press 'R' to Restart"));
		DrawText(RestartTextYouWin, FLinearColor::White, HorizontalCenterOfScreen - 300.0f, VerticalCenterOfScreen + 70.0f, nullptr, 2.5f, false);
	}
}

/*
 * Updates Percentage of Enemies Remaining
 */
void AShooterHUD::UpdateEnemyPercentage(float StatePercentage)
{
	HUDEnemyPercentage = FMath::Clamp(StatePercentage, 0.0f, 1.0f);
}

/*
 * Updates current wave number
 */
void AShooterHUD::UpdateWaveNumber(int StateWaveNumber)
{
	HUDWaveNumber = StateWaveNumber;
}

/*
 * Updates Player's current health
 */
void AShooterHUD::UpdatePlayerHealth(float StateHealthPercentage)
{
	HUDPlayerHealth = FMath::Clamp(StateHealthPercentage, 0.0f, 1.0f);
}

/*
 * Updates whether the player has died or not
 */
void AShooterHUD::UpdateGameState(int StateGameState)
{
	HUDGameOver = StateGameState;
}

/*
 * Updates whether the player has beaten all 5 wave's or not
 */
void AShooterHUD::UpdateWinStatus(int StateYouWon)
{
	HUDYouWon = StateYouWon;
}
