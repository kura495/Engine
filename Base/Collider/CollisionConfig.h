#pragma once
#include<cstdint>

namespace ColliderTag{
	const uint32_t Player = 0b1;
	const uint32_t Enemy = 0b1 << 1;
	const uint32_t Weapon = 0b1 << 2;
	const uint32_t BossBullet = 0b1 << 3;
	const uint32_t MoveFloor = 0b1 << 4;
	const uint32_t Goal = 0b1 << 5;
	const uint32_t Box = 0b1 << 6;
	const uint32_t EnemyAttack = 0b1 << 7;
	const uint32_t Floor = 0b1 << 7;
}


enum ColliderType {
	Circle,
	Box,
	OBBox,
};