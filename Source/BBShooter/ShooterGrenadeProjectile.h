#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ShooterGrenadeProjectile.generated.h"

UCLASS()
class BBSHOOTER_API AShooterGrenadeProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AShooterGrenadeProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Projectile")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	UParticleSystem* ParticleSystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "Explosion")
	class URadialForceComponent* RadialForceComponent;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	USoundBase* ProjectileSound;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void GrenadeExplode();

	virtual void Tick(float DeltaTime) override;
	
	FTimerHandle TimerHandleExplosion;

protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned
};
