// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "MonsterAnimInstance.h"
#include "Monster.generated.h"

UCLASS()
class SLIDESCROLL_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EMonsterState GetMonsterState();

protected:
	virtual void IdleReady(float DeltaTime);
	virtual void IdleCombat(float DeltaTime);
	virtual void FollowPlayerChar(float DeltaTime);
	virtual void AttackPlayerChar(float DeltaTime);
	virtual void Death(float DeltaTime);

	virtual void SetStatus();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void ChangeDamageColor();
	void UpdateGameInfo();

	UFUNCTION()
	void OnColStartAttack();
	UFUNCTION()
	void OnColEndAttack();
	UFUNCTION()
	void OnBeginWeaponOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere, Category = Attack)
	UCapsuleComponent* WeaponCollision;
	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* AttackAnim;
	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* DeathAnim;

	UMonsterAnimInstance* AnimInstance;

protected:
	EMonsterState CurrentState;
	float HP;
	float AttackPower;
	float AggroRange;
	float AttackRange;

	float DistanceFromPlayerChar;
	FVector DirectionToPlayerChar;
	bool IsDeathPlayerChar;
};
