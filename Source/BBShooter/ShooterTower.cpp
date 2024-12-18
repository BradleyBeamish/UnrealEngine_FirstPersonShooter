#include "ShooterTower.h"
#include "ShooterCharacter.h"
#include "ShooterGameStateBase.h"
#include "Kismet/GameplayStatics.h"

/*
 * Sets default values
 */
AShooterTower::AShooterTower()
{
	PrimaryActorTick.bCanEverTick = true;

	// MESH COMPONENT
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetupAttachment(RootComponent);

	// PAWN SENSING COMPONENT
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->bEnableSensingUpdates = true;
	PawnSensingComponent->SightRadius = 1500.0f;
	PawnSensingComponent->SensingInterval = 0.1f; // Changes how fast the tower rotates towards player
	PawnSensingComponent->SetPeripheralVisionAngle(180.0f);
}

/*
 * Called when the game starts, and sets up pawn sensing component
 */
void AShooterTower::BeginPlay()
{
	Super::BeginPlay();

	OriginalTransform = GetActorTransform(); // Save original world placement, as the grenades move the tower from its place

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AShooterTower::OnPawnSeen);
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error setting up PawnSensingComponent"));
	}

	AShooterGameStateBase* GameState = GetWorld()->GetGameState<AShooterGameStateBase>();
	if (GameState)
	{
		GameState->CurrentEnemyCount++;
	}
}

/*
 * Called every frame
 */
void AShooterTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
 * When the tower sees a pawn, it ensures it is a character (and not moving enemy)
 * It then rotates to follow the player
 * When the tower is facing the player and cooldown is off, it will fire
 */
void AShooterTower::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn)
	{
		AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(SeenPawn);
		if (ShooterCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Character"));

			// Find rotation needed to be facing the player/character
			FVector Direction = (ShooterCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
			LookAtRotation.Pitch = 0.0f;
			LookAtRotation.Roll = 0.0f;

			// Rotate towards character
			FRotator CurrentRotation = GetActorRotation();
			FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, GetWorld()->GetDeltaSeconds(), 10.0f);
			SetActorRotation(InterpolatedRotation);

			// Tower needs to face character to shoot
			if (FMath::Abs(CurrentRotation.Yaw - LookAtRotation.Yaw) < 10.0f)
			{
				// The tower can move every 0.1s (pawn sensing), but can only fire every 3.0s
				float CurrentTime = GetWorld()->GetTimeSeconds();
				if (CurrentTime - PreviousProjectileFire >= ProjectileCooldown)
				{
					FireRegularProjectile(ShooterCharacter);
					PreviousProjectileFire = CurrentTime;
				}
			}
		}
	}
}

/*
 * Sets the player within sensing range as the target, and shoots a regular projectile at them
 */
void AShooterTower::FireRegularProjectile(AActor* TargetCharacter)
{
	if (RegularProjectileClass && TargetCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Firing Regular Projectile at %s"), *TargetCharacter->GetName());

		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 400.0f;
		FRotator SpawnRotation = (TargetCharacter->GetActorLocation() - SpawnLocation).Rotation();

		// Set spawn parameters
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the projectile
		AShooterRegularProjectile* Projectile = GetWorld()->SpawnActor<AShooterRegularProjectile>(RegularProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

		if (Projectile)
		{
			Projectile->SetTargetCharacter(TargetCharacter);
		}

		// Play Sound
		UGameplayStatics::PlaySoundAtLocation(this, ProjectileSound, GetActorLocation());
	}
}

/*
 * TakeDamage is only called for Grenades, therefore no need to check if it's a grenade
 */
float AShooterTower::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Tower Took Damage"));
	DisableTower();
	return DamageTaken;
}

/*
 * When the tower gets hit by a grenade, it becomes disable
 * This is used instead of respawning the towers individually every match
 */
void AShooterTower::DisableTower()
{
	UE_LOG(LogTemp, Warning, TEXT("Tower Disabled"));
	
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	PawnSensingComponent->SetActive(false);
	
	AShooterGameStateBase* GameState = GetWorld()->GetGameState<AShooterGameStateBase>();
	if (GameState)
	{
		GameState->EnemyDestroyed();
	}
}

/*
 * Re-enables tower after being disabled
 * Used at the start of every round
 */
void AShooterTower::EnableTower()
{
	UE_LOG(LogTemp, Warning, TEXT("Tower Enabled"));
	
	SetActorTransform(OriginalTransform); // Move tower back to where it was before grenade explosion
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	PawnSensingComponent->SetActive(true);
	
	AShooterGameStateBase* GameState = GetWorld()->GetGameState<AShooterGameStateBase>();
	if (GameState)
	{
		GameState->CurrentEnemyCount++;
	}
}

/*
 * Resets the tower ("Destroys" it, then brings it back)
 */
void AShooterTower::ResetTower()
{
	DisableTower();
	EnableTower();
}
