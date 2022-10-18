#pragma once

#include "Enum.generated.h"

UENUM()
enum class EGamePlayState : uint8
{
	Ready,
	Play,
	StageClear,
	PlayerDeath,
};

UENUM()
enum class EMonsterState : uint8
{
	IdleReady,
	IdleCombat,
	FollowPlayerChar,
	AttackPlayerChar,
	SkillAttackPlayerChar,
	Death,
};