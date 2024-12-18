#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterWalls.generated.h"

UCLASS()
class BBSHOOTER_API AShooterWalls : public AActor
{
	GENERATED_BODY()
	
public:	
	AShooterWalls(); // Sets default values for this actor's properties

	virtual void Tick(float DeltaTime) override; // Called every frame

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShooterWalls")
	UStaticMeshComponent* WallMesh;

protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned
};
