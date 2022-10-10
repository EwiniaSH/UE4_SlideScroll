#pragma once

#include "Enum.generated.h"

UENUM()
enum class EMonsterState : uint8
{
	IdleReady,
	IdleCombat,
	FollowPlayerChar,
	AttackPlayerChar,
	Death,
};