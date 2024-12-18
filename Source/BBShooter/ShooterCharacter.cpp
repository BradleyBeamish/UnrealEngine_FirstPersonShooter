#include "ShooterCharacter.h"

#include "ShooterGameStateBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterRegularProjectile.h"
#include "ShooterGrenadeProjectile.h"
#include "ShooterMovingEnemy.h"

/*
 * Sets the default values
 */
AShooterCharacter::AShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// CREATE SPRING-ARM, ATTACH TO ROOT
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 550.0f;
	SpringArm->bUsePawnControlRotation = true;

	// CREATE CAMERA AND ATTACH TO SPRING-ARM
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// MESH COMPONENT
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->OnComponentHit.AddDynamic(this, &AShooterCharacter::OnHit);
	MeshComponent->SetupAttachment(RootComponent);
}

/*
 * Called when the game starts
 */
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	OriginalTransform = GetActorTransform();
}

/*
 * Called every frame
 */
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
 * Bind Keybindings to KBM Input:
 * W -> Move Forward
 * S -> Move Backward
 * A -> Turn Left
 * D -> Turn Right
 * Left-Click -> Shoot Regular Projectile
 * Right-Click -> Shoot Grenade Projectile
 * Pan Camera -> Move Mouse
 */
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Character Movement
	PlayerInputComponent->BindAxis("Move", this, &AShooterCharacter::Move);
	PlayerInputComponent->BindAxis("Turn", this, &AShooterCharacter::Turn);

	// Camera Movement
	PlayerInputComponent->BindAxis("MoveCamera", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnCamera", this, &APawn::AddControllerYawInput);

	// Projectiles
	PlayerInputComponent->BindAction("RegularProjectile", IE_Pressed, this, &AShooterCharacter::FireRegularProjectile);
	PlayerInputComponent->BindAction("GrenadeProjectile", IE_Pressed, this, &AShooterCharacter::FireGrenadeProjectile);

	// Restart
	PlayerInputComponent->BindAction("RestartGame", IE_Pressed, this, &AShooterCharacter::RestartGame);
}

/*
 * Move Forward or Backward
 */
void AShooterCharacter::Move(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value * 500.0f * GetWorld()->GetDeltaSeconds());
	}
}

/*
 * Turn Left or Right
 */
void AShooterCharacter::Turn(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerYawInput(Value * 100.0f * GetWorld()->GetDeltaSeconds());
	}
}
/*
 * Fires a Regular Projectile
 * Spawns it right in front of the player, then the Regular Projectile
 *  class handles it's forward movement
 */
void AShooterCharacter::FireRegularProjectile()
{
	if (RegularProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Firing Regular Projectile"));
		
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
		FRotator SpawnRotation = GetActorRotation();

		// Set spawn parameters
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the projectile
		GetWorld()->SpawnActor<AShooterRegularProjectile>(RegularProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

		// Play Sound
		UGameplayStatics::PlaySoundAtLocation(this, ProjectileSound, GetActorLocation());
	}
}

/*
 * Fire a Grenade Projectile
 * Spawned right in front of the player, sticks to whatever it hits
 */
void AShooterCharacter::FireGrenadeProjectile()
{
	if (GrenadeProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Firing Grenade Projectile"));

		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
		FRotator SpawnRotation = GetActorRotation();

		// Set spawn parameters
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the projectile
		GetWorld()->SpawnActor<AShooterGrenadeProjectile>(GrenadeProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		
		// Play Sound
		UGameplayStatics::PlaySoundAtLocation(this, ProjectileSound, GetActorLocation());
	}
}

/*
 * Bound to "R" Key
 * Tells the State Class to start the restart procedure
 */
void AShooterCharacter::RestartGame()
{
	AShooterGameStateBase* GameState = GetWorld()->GetGameState<AShooterGameStateBase>();
	
	if (GameState)
	{
		GameState->RestartGame();
	}
}

/*
 * Character looses some amount of health
 * This will be from a tower projectile, or a moving enemy hit
 */
float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount == 100.0f)
	{
		return DamageAmount; // Damage coming from a grenade, ignore
	}
	
	Health -= DamageAmount;
	
	UE_LOG(LogTemp, Warning, TEXT("Player Took Damage, Current Health: %d"), Health);

	AShooterGameStateBase* GameState = GetWorld()->GetGameState<AShooterGameStateBase>();
	if (GameState)
	{
		GameState->SetPlayerHealth(Health);
	}

	if (Health <= 0)
	{
		// Character has died, temporarily disable until restart
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		MeshComponent->SetVisibility(false);
	}

	return DamageAmount;
}

/*
 * Player takes damage when Hit by a Moving Enemy
 */
void AShooterCharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Player was hit by %s"), *OtherActor->GetName());

	if (OtherActor && Cast<AShooterMovingEnemy>(OtherActor))
	{
		UGameplayStatics::ApplyDamage(this, 10, GetInstigatorController(), this, UDamageType::StaticClass());
	}
}

/*
 * Called on a reset (R Key)
 * Brings the character to its original Spawn Point, re-enables features
 */
void AShooterCharacter::ResetCharacter()
{
	SetActorTransform(OriginalTransform);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	MeshComponent->SetVisibility(true);

	Health = 100.0f;

	AShooterGameStateBase* GameState = GetWorld()->GetGameState<AShooterGameStateBase>();
	if (GameState)
	{
		GameState->SetPlayerHealth(Health);
	}
}
