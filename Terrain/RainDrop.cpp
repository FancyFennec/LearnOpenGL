#include "RainDrop.h"
#include <iostream>

RainDrop::RainDrop(int width, int height, std::vector<float>& heightMap) :
	width(width),
	height(height),
	heightMap(heightMap)
{
	pos[0] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (float) (width -1)));
	pos[1] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (float) (height - 1)));
}


RainDrop::~RainDrop()
{
}

void RainDrop::computeGradient()
{
	/*std::cout << '\n' << pos[0];
	std::cout << '\n' << std::floorf(pos[0]);
	std::cout << '\n' << std::ceilf(pos[0]);
	std::cout << '\n' << pos[1];
	std::cout << '\n' << std::floorf(pos[1]);
	std::cout << '\n' << std::ceilf(pos[1]);
	std::cout << '\n' << heightMap.size();*/

	int p_x = std::floorf(pos[0]);
	int p_x1 = std::ceilf(pos[0]);
	int p_y = std::floorf(pos[1]);
	int p_y1 = std::ceilf(pos[1]);

	float u = pos[0] - p_x;
	float v = pos[1] - p_y;

	grad[0] = (heightMap[getIndex(p_x1, p_y)] - heightMap[getIndex(p_x, p_y)]) * (1 - v)
		+ (heightMap[getIndex(p_x1, p_y1)] - heightMap[getIndex(p_x, p_y1)]) * v;
	grad[1] = (heightMap[getIndex(p_x, p_y1)] - heightMap[getIndex(p_x, p_y)]) * (1 - u)
		+ (heightMap[getIndex(p_x1, p_y1)] - heightMap[getIndex(p_x1, p_y)]) * u;

}

void RainDrop::computeDirection()
{
	dir[0] = dir[0] * p_inertia - grad[0] * (1 - p_inertia);
	dir[1] = dir[1] * p_inertia - grad[1] * (1 - p_inertia);

	if (dir[0] == 0 && dir[1] == 0) {
		dir[0] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
		dir[1] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
	}

	float norm = sqrtf(dir[0] * dir[0] + dir[1] * dir[1]);

	dir[0] /= norm;
	dir[1] /= norm;
}

void RainDrop::computePosition()
{
	oldPos = pos;

	pos[0] += dir[0]; 
	pos[1] += dir[1];
}

void RainDrop::computeHeight()
{
	float newHeight = 0.0f;

	newHeight += heightMap[getIndex(std::floorf(pos[0]), std::floorf(pos[1]))];
	newHeight += heightMap[getIndex(std::ceilf(pos[0]), std::floorf(pos[1]))];
	newHeight += heightMap[getIndex(std::floorf(pos[0]), std::ceilf(pos[1]))];
	newHeight += heightMap[getIndex(std::ceilf(pos[0]), std::ceilf(pos[1]))];

	newHeight /= 4.0f;

	terrain_height_diff = newHeight - terrain_height;
	terrain_height_diff = terrain_height_diff > 0 ? terrain_height_diff : - terrain_height_diff;
	terrain_height = newHeight;
}

void RainDrop::computeCapacity()
{
	capacity = max(-terrain_height_diff, p_minSlope) * vel * water *p_capacity;
}

void RainDrop::computeErosionStep()
{

	if (sediment > capacity) {
		float deposed_sediment = (sediment - capacity) * p_deposition / 4.0f;

		heightMap[getIndex(std::floorf(pos[0]), std::floorf(pos[1]))] += deposed_sediment;
		heightMap[getIndex(std::floorf(pos[0]), std::ceilf(pos[1]))] += deposed_sediment;
		heightMap[getIndex(std::ceilf(pos[0]), std::floorf(pos[1]))] += deposed_sediment;
		heightMap[getIndex(std::ceilf(pos[0]), std::ceilf(pos[1]))] += deposed_sediment;

		sediment -= deposed_sediment;
	} else {
		float eroded_sediment = min((capacity - sediment) * p_erosion, terrain_height_diff) / 4.0f;

		heightMap[getIndex(std::floorf(oldPos[0]), std::floorf(oldPos[1]))] -= eroded_sediment;
		heightMap[getIndex(std::floorf(oldPos[0]), std::ceilf(oldPos[1]))] -= eroded_sediment;
		heightMap[getIndex(std::ceilf(oldPos[0]), std::floorf(oldPos[1]))] -= eroded_sediment;
		heightMap[getIndex(std::ceilf(oldPos[0]), std::ceilf(oldPos[1]))] -= eroded_sediment;

		sediment += eroded_sediment;
	}
}

void RainDrop::computeVelocity()
{
	vel = sqrt(vel * vel + terrain_height_diff * p_gravity);
}

void RainDrop::computeWater()
{
	water *= (1 - p_evaporation) * water;
}

int RainDrop::computeStep()
{
	computeGradient();
	computeDirection();
	computePosition();
	if (pos[0] < 0 || pos[0] > width - 1 || pos[1] < 0 || pos[1] > height - 1) {
		return 0;
	}
	computeHeight();
	computeCapacity();
	computeErosionStep();
	computeVelocity();
	computeWater();

	return 1;
}