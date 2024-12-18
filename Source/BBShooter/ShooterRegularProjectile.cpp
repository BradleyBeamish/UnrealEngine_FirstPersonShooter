#include "ShooterRegularProjectile.h"
#include "ShooterCharacter.h"
#include "ShooterMovingEnemy.h"
#include "Kismet/GameplayStatics.h"

/*
 * Sets default values
 */
AShooterRegularProjectile::AShooterRegularProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 5.0f;

	// SPHERE COLLISIONS
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("BlockAll");
	CollisionComp->OnComponentHit.AddDynamic(this, &AShooterRegularProjectile::OnHit);
	RootComponent = CollisionComp;
	
	// PROJECTILE MOVEMENT
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->bShouldBounce = true;

	// MESH COMPONENT
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}

/*
 * Called when spawned
 */
void AShooterRegularProjectile::BeginPlay()
{
	Super::BeginPlay();
}

/*
 * Called every frame
 * If the projectile has a target (Tower -> Player), go in that direction
 */
void AShooterRegularProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetCharacter)
	{
		FVector Direction = (TargetCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
	}
}

/*
 * Projectile hits something
 */
void AShooterRegularProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Hits Moving Enemy, Apply 50 damage
	if (AShooterMovingEnemy* MovingEnemy = Cast<AShooterMovingEnemy>(OtherActor))
	{
		UGameplayStatics::ApplyDamage(MovingEnemy, MovingEnemyDamageAmount, GetInstigatorController(), this, UDamageType::StaticClass());
		Destroy(); // Projectile will disappear on hit
		
	}

	// Hits Player, Apply 10 Damage
	else if (AShooterCharacter* Player = Cast<AShooterCharacter>(OtherActor))
	{
		UGameplayStatics::ApplyDamage(Player, PlayerDamageAmount, GetInstigatorController(), this, UDamageType::StaticClass());
		Destroy();
	}
}

/*
 * Set a character as a target (used by tower)
 */
void AShooterRegularProjectile::SetTargetCharacter(AActor* NewTarget)
{
	TargetCharacter = NewTarget;
}
