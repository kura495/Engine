#pragma once
struct Vector2 final {
	float x;
	float y;
	Vector2 operator +=(Vector2 other) {
		this->x = this->x + other.x;
		this->y = this->y + other.y;
		return { this->x,this->y};
	}
};