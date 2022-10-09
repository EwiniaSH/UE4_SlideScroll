// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyCharacterAnimInstance.generated.h"

DECLARE_DELEGATE(FOnColStartAttackDelegate);
DECLARE_DELEGATE(FOnColEndAttackDelegate);

/**
 * 
 */
UCLASS()
class SLIDESCROLL_API UMyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void AnimNotify_OnColStartAttack();
	UFUNCTION()
	void AnimNotify_OnColEndAttack();

	FOnColStartAttackDelegate OnColStartAttack;
	FOnColEndAttackDelegate OnColEndAttack;
};
