#include "ShooterGrenadeProjectile.h"
#include "ShooterRegularProjectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

/*
 * Sets default values
 */
AShooterGrenadeProjectile::AShooterGrenadeProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// SPHERE COLLISIONS
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(5.0f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAll"));
	SphereComponent->OnComponentHit.AddDynamic(this, &AShooterGrenadeProjectile::OnHit);
	RootComponent = SphereComponent;
	
	// PROJECTILE MOVEMENT
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = SphereComponent;
	ProjectileMovementComponent->InitialSpeed = 4000.0f;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

	// MESH COMPONENT
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	// RADIAL FORCE COMPONENT
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->Radius = 300.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bAutoActivate = false;
	RadialForceComponent->bIgnoreOwningActor = true;
}

/*
 * Called when the game starts
 */
void AShooterGrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();
}

/*
 * Called every frame
 */
void AShooterGrenadeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
 * When the grenade hits something, it will attach itself and stop moving, then explode after 2 seconds
 */
void AShooterGrenadeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		// Attach to hit actor
		AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);

		// Stop grenade movement
		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->StopMovementImmediately();
		}

		// Start timer for explosion
		GetWorldTimerManager().SetTimer(TimerHandleExplosion, this, &AShooterGrenadeProjectile::GrenadeExplode, 2.0f, false);
	}
}

/*
 * Explode Grenade (Includes damage, sound, and particles)
 */
void AShooterGrenadeProjectile::GrenadeExplode()
{
	// Blast Radius + Radial Damage
	RadialForceComponent->FireImpulse();
	UGameplayStatics::ApplyRadialDamage(this, 100, GetActorLocation(), 300, UDamageType::StaticClass(), TArray<AActor*>(), this);

	// Explosion sound + effect
	UGameplayStatics::PlaySoundAtLocation(this, ProjectileSound, GetActorLocation()); 
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystemComponent, GetActorLocation());

	Destroy();
}
