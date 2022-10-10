// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyCharacterAnimInstance.h"
#include "MonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SLIDESCROLL_API UMonsterAnimInstance : public UAnimInstance
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
