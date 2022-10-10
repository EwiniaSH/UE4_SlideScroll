// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterAnimInstance.h"
#include "BossMonsterAnimInstance.generated.h"

DECLARE_DELEGATE(FOnColStartSkillDelegate);
DECLARE_DELEGATE(FOnColEndSkillDelegate);

/**
 * 
 */
UCLASS()
class SLIDESCROLL_API UBossMonsterAnimInstance : public UMonsterAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
		void AnimNotify_OnColStartSkill();
	UFUNCTION()
		void AnimNotify_OnColEndSkill();

	FOnColStartSkillDelegate OnColStartSkill;
	FOnColEndSkillDelegate OnColEndSkill;
};
