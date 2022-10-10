// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonsterAnimInstance.h"

void UBossMonsterAnimInstance::AnimNotify_OnColStartSkill()
{
	OnColStartSkill.Execute();
}

void UBossMonsterAnimInstance::AnimNotify_OnColEndSkill()
{
	OnColEndSkill.Execute();
}