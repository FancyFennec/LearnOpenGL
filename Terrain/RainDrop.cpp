#include "RainDrop.h"

RainDrop::RainDrop(unsigned int width, unsigned int height, std::vector<float> &heightMap) :
	width(width),
	height(height),
	heightMap(heightMap)
{
	pos[0] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (float) width));
	pos[1] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (float) height));
}


RainDrop::~RainDrop()
{
}

void RainDrop::computeGradient()
{
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

	float norm = sqrt(dir[0] * dir[0] + dir[1] * dir[1]);

	dir[0] /= norm;
	dir[1] /= norm;
}

void RainDrop::computePosition()
{
	oldPos = pos;
	pos[0] += dir[0]; pos[1] += dir[1];
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
	terrain_height = newHeight;
}

void RainDrop::computeCapacity()
{
	capacity = max(-terrain_height_diff, p_minSlope) * vel * water *p_capacity;
}

void RainDrop::computeErosionStep()
{
	int xPos, yPos;

	if (sediment > capacity) {
		xPos = oldPos[0] - std::floorf(oldPos[0]) < std::ceilf(oldPos[0]) - oldPos[0] ? 
			std::floorf(oldPos[0]) : std::ceilf(oldPos[0]);
		yPos = oldPos[1] - std::floorf(oldPos[1]) < std::ceilf(oldPos[1]) - oldPos[1] ? 
			std::floorf(oldPos[1]) : std::ceilf(oldPos[1]);

		heightMap[getIndex(xPos, yPos)] += (sediment - capacity) * p_deposition;
	} else {
		xPos = pos[0] - std::floorf(pos[0]) < std::ceilf(pos[0]) - pos[0] ?
			std::floorf(pos[0]) : std::ceilf(oldPos[0]);
		yPos = pos[1] - std::floorf(pos[1]) < std::ceilf(pos[1]) - pos[1] ?
			std::floorf(pos[1]) : std::ceilf(pos[1]);

		heightMap[getIndex(xPos, yPos)] -= min((capacity - sediment) * p_erosion, -terrain_height_diff);
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
	if (pos[0] < 0 || pos[0] > width || pos[1] < 0 || pos[1] >height) {
		return 0;
	}
	computeHeight();
	computeCapacity();
	computeErosionStep();
	computeVelocity();
	computeWater();
	
	return 1;
}


