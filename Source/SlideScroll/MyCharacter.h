// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "MyCharacterAnimInstance.h"
#include "MyCharacter.generated.h"

UCLASS()
class SLIDESCROLL_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

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

private:
	void SetStatus();
	void Movement(float Value);
	void Attack();
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void ChangeDamageColor();

	UFUNCTION()
	void OnColStartAttack();
	UFUNCTION()
	void OnColEndAttack();
	UFUNCTION()
	void OnBeginWeaponOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = Attack)
	UCapsuleComponent* WeaponCollision;
	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* AttackAnim;
	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* JumpAnim;
	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* DeathAnim;

	UMyCharacterAnimInstance* AnimInstance;

private:
	float HP;
	float AttackPower;

public:
	bool IsDeath;
};
