#pragma once

#include "CoreMinimal.h"
#include "ShooterGrenadeProjectile.h"
#include "ShooterRegularProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class BBSHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AShooterCharacter(); // Sets default values for this character's properties
	
	virtual void Tick(float DeltaTime) override; // Called every frame
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; // Called to bind functionality to input

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AShooterRegularProjectile> RegularProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AShooterGrenadeProjectile> GrenadeProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	USoundBase* ProjectileSound;

	// Functions bound to KBM
	void Move(float Value);
	void Turn(float Value);
	void FireRegularProjectile();
	void FireGrenadeProjectile();
	void RestartGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterHealth")
	int Health = 100.0f;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ResetCharacter();

	FTransform OriginalTransform;
	
protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned
};
