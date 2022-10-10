// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"

void UMonsterAnimInstance::AnimNotify_OnColStartAttack()
{
	OnColStartAttack.Execute();
}

void UMonsterAnimInstance::AnimNotify_OnColEndAttack()
{
	OnColEndAttack.Execute();
}