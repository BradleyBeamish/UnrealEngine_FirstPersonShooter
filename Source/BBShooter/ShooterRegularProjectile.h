#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ShooterRegularProjectile.generated.h"

UCLASS()
class BBSHOOTER_API AShooterRegularProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AShooterRegularProjectile(); // Sets default values for this actor's properties

	virtual void Tick(float DeltaTime) override; // Called every frame

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Projectile")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	int MovingEnemyDamageAmount = 50.0f;

	int PlayerDamageAmount = 10.0f;

	UPROPERTY()
	AActor* TargetCharacter;

	void SetTargetCharacter(AActor* NewTarget);

protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned
};
