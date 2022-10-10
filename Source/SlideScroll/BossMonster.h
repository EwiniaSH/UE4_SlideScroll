// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "BossMonster.generated.h"

/**
 * 
 */
UCLASS()
class SLIDESCROLL_API ABossMonster : public AMonster
{
	GENERATED_BODY()

public:
	ABossMonster();

protected:
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void FollowPlayerChar(float DeltaTime) override;
	virtual void AttackPlayerChar(float DeltaTime) override;
	void SkillAttackPlayerChar(float DeltaTime);

	virtual void SetStatus() override;

private:
	void ChangeSkillColor();

	UFUNCTION()
		void OnColStartSkill();
	UFUNCTION()
		void OnColEndSkill();
	UFUNCTION()
		void OnBeginSkillOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere, Category = Attack)
	UCapsuleComponent* SkillCollision;
	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* SkillReadyAnim;
	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimMontage* SkillAttackAnim;

private:
	float SkillPower;
	float SkillCoolTime;
	float SkillDeltaTime;
};
