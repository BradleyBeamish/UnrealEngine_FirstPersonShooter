#include "ShooterMovingEnemy.h"

#include "AIController.h"
#include "ShooterCharacter.h"
#include "ShooterGameStateBase.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/StaticMeshComponent.h"

/*
 * Sets default values
 */
AShooterMovingEnemy::AShooterMovingEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// MESH COMPONENT
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetupAttachment(RootComponent);

	// PAWN SENSING COMPONENT
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->bEnableSensingUpdates = true;
	PawnSensingComponent->SightRadius = 1000.0f;
	PawnSensingComponent->SensingInterval = 0.2f;
	PawnSensingComponent->SetPeripheralVisionAngle(180.0f);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; // Already set in blueprint, however adding this here fixes enemies spawned by the spawner
}

/*
 * Called when the game starts, sets up the pawn sensing component
 */
void AShooterMovingEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AShooterMovingEnemy::OnPawnSeen);
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error setting up PawnSensingComponent"));
	}
}

/*
 * Called every frame
 */
void AShooterMovingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
 * When the Moving Enemy senses a pawn, it ensures it is a character,
 *  then uses the built in AIController to move towards the character.
 */
void AShooterMovingEnemy::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn)
	{
		AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(SeenPawn);
		if (ShooterCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pawn seen: %s"), *SeenPawn->GetName());
		
			// The Moving Enemy uses the default AIController
			AAIController* AIController = Cast<AAIController>(GetController());
		
			if (AIController)
			{
				AIController->MoveToActor(SeenPawn, 10.0f);
			}
		}
	}
}

/*
 * Moving Enemies are designed to be taken down in 1 grenade projectile hit, or 2 regular projectile hits
 */
float AShooterMovingEnemy::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageTaken;
	
	UE_LOG(LogTemp, Warning, TEXT("Moving Enemy Took Damage, Current Health: %d"), Health);

	if (Health <= 0)
	{
		// Decrease World Player Count
		AShooterGameStateBase* GameState = GetWorld()->GetGameState<AShooterGameStateBase>();
		if (GameState)
		{
			GameState->EnemyDestroyed();
		}
		
		Destroy();
	}

	return DamageTaken;
}
