#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Modules/ModuleManager.h"
#include "Perception/PawnSensingComponent.h"
#include "ShooterMovingEnemy.generated.h"

UCLASS()
class BBSHOOTER_API AShooterMovingEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:	
	AShooterMovingEnemy();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovingEnemyAI")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovingEnemyAI")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovingEnemyAI")
	int Health = 100.0f;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);
	
	virtual float TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
};
