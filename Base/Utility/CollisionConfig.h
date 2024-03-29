﻿#pragma once
#include<cstdint>

//プレイヤー陣営
const uint32_t kCollitionAttributePlayer = 0b1;
//敵陣営
const uint32_t kCollitionAttributeEnemy = 0b1 << 1;
//床
const uint32_t kCollitionAttributeFloor = 0b1 << 2;
const uint32_t kCollitionAttributeMoveFloor = 0b1 << 3;
//ゴール
const uint32_t kCollitionAttributeGoal = 0b1 <<4;
//箱
const uint32_t kCollitionAttributeBox = 0b1 << 5;
//武器
const uint32_t kCollitionAttributeWeapon = 0b1 << 6;
//SearchPoint
const uint32_t kCollitionAttributeSearchPoint = 0b1 << 7;


enum ColliderType {
	Circle,
	Box,
	OBBox,
};