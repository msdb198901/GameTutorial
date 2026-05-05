#pragma once

class WaterTile
{
public:
	static const float TILE_SIZE;

	WaterTile(float centerX, float centerZ, float height) {
		this->x = centerX;
		this->z = centerZ;
		this->height = height;
	}

	float GetHeight() {
		return height;
	}

	float GetX() {
		return x;
	}

	float GetZ() {
		return z;
	}

private:
	float height;
	float x, z;
};
