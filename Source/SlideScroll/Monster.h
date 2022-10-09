// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enum.h"
#include "Monster.generated.h"

UCLASS()
class SLIDESCROLL_API AMonster : public ACharacter
{
	GENERATED_BODY()

	static const float AGGRO_RANGE;

public:
	// Sets default values for this character's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Idle();
	void FollowPlayerChar();
	void AttackPlayerChar();

	void SetStatus();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void ChangeDamageColor();
	void SetDistanceAndDirection();

public:
	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* DeathAnim;

private:
	EMonsterState CurrentState;
	float HP;
	float AttackPower;
	float DistanceFromPlayerChar;
	FVector DirectionToPlayerChar;
};
