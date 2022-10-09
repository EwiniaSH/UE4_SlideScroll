// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterAnimInstance.h"

void UMyCharacterAnimInstance::AnimNotify_OnColStartAttack()
{
	OnColStartAttack.Execute();
}

void UMyCharacterAnimInstance::AnimNotify_OnColEndAttack()
{
	OnColEndAttack.Execute();
}