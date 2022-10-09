#pragma once

#include "Enum.generated.h"

UENUM()
enum class EMonsterState : uint8
{
	Idle,
	FollowPlayerChar,
	AttackPlayerChar,
};