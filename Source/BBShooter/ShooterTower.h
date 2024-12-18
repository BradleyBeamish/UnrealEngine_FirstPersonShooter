#pragma once

#include "CoreMinimal.h"
#include "ShooterRegularProjectile.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "ShooterTower.generated.h"

UCLASS()
class BBSHOOTER_API AShooterTower : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterTower();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower")
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AShooterRegularProjectile> RegularProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	USoundBase* ProjectileSound;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	void FireRegularProjectile(AActor* TargetCharacter);

	virtual float TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	float PreviousProjectileFire;
	
	float ProjectileCooldown = 3.0f;

	void DisableTower();
	
	void EnableTower();

	void ResetTower();

	FTransform OriginalTransform;
	
protected:
	virtual void BeginPlay() override;
};
