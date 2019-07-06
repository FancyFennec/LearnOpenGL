#include "RainDrop.h"
#include <iostream>

RainDrop::RainDrop(int width, int height, std::vector<float>& heightMap) :
	width(width),
	height(height),
	heightMap(heightMap)
{
	pos[0] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (float) (width -1)));
pos[1] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (float)(height - 1)));
}


RainDrop::~RainDrop()
{
}

void RainDrop::sortIndices(std::vector<int>& indices)
{
	if (heightMap[getIndex(indices[0], indices[1])] > heightMap[getIndex(indices[4], indices[5])]) {
		std::iter_swap(indices.begin(), indices.begin() + 4);
		std::iter_swap(indices.begin() + 1, indices.begin() + 5);
	}
	if (heightMap[getIndex(indices[2], indices[3])] > heightMap[getIndex(indices[6], indices[7])]) {
		std::iter_swap(indices.begin() + 2, indices.begin() + 6);
		std::iter_swap(indices.begin() + 3, indices.begin() + 7);
	}
	if (heightMap[getIndex(indices[0], indices[1])] > heightMap[getIndex(indices[2], indices[3])]) {
		std::iter_swap(indices.begin(), indices.begin() + 2);
		std::iter_swap(indices.begin() + 1, indices.begin() + 3);
	}
	if (heightMap[getIndex(indices[4], indices[5])] > heightMap[getIndex(indices[6], indices[7])]) {
		std::iter_swap(indices.begin() + 4, indices.begin() + 6);
		std::iter_swap(indices.begin() + 5, indices.begin() + 7);
	}
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
	terrain_height = newHeight;
}

void RainDrop::computeCapacity()
{
	capacity = max(-terrain_height_diff, p_minSlope) * vel * water *p_capacity;
}

void RainDrop::computeErosionStep()
{
	int posX = std::floorf(oldPos[0]);
	int posX1 = std::ceilf(oldPos[0]);
	int posY = std::floorf(oldPos[1]);
	int posY1 = std::ceilf(oldPos[1]);

	if (terrain_height_diff > 0) {
		float heightDiff = 0.0f;
		std::vector<int> indices = { posX, posY, posX1, posY, posX, posY1, posX1, posY1 };
		sortIndices(indices);

		if (heightMap[getIndex(indices[0], indices[1])] + sediment <=
			heightMap[getIndex(indices[2], indices[3])]){

			heightMap[getIndex(indices[0], indices[1])] += sediment;
			sediment = 0;
		} else{
			heightDiff = heightMap[getIndex(indices[2], indices[3])] - heightMap[getIndex(indices[0], indices[1])];
			heightMap[getIndex(indices[0], indices[1])] += heightDiff;
			sediment -= heightDiff;

			if (heightMap[getIndex(indices[2], indices[3])] + sediment / 2.0f <=
				heightMap[getIndex(indices[4], indices[5])]) {

				heightMap[getIndex(indices[0], indices[1])] += sediment / 2.0f;
				heightMap[getIndex(indices[2], indices[3])] += sediment / 2.0f;
				sediment = 0;
			} else {
				heightDiff = heightMap[getIndex(indices[4], indices[5])] - heightMap[getIndex(indices[2], indices[3])];
				heightMap[getIndex(indices[0], indices[1])] += heightDiff;
				heightMap[getIndex(indices[2], indices[3])] += heightDiff;
				sediment -= heightDiff * 2;

				if (heightMap[getIndex(indices[4], indices[5])] + sediment / 3.0f <=
					heightMap[getIndex(indices[6], indices[7])]) {

					heightMap[getIndex(indices[0], indices[1])] += sediment / 3.0f;
					heightMap[getIndex(indices[2], indices[3])] += sediment / 3.0f;
					heightMap[getIndex(indices[2], indices[3])] += sediment / 3.0f;
					sediment = 0;
				}
				else {
					heightDiff = heightMap[getIndex(indices[6], indices[7])] - heightMap[getIndex(indices[4], indices[5])];
					heightMap[getIndex(indices[0], indices[1])] += heightDiff;
					heightMap[getIndex(indices[2], indices[3])] += heightDiff;
					heightMap[getIndex(indices[4], indices[5])] += heightDiff;
					sediment -= heightDiff * 3;
				}
			}
		}
	} else {

		float totalHeight = 0.0f;
		totalHeight += heightMap[getIndex(posX, posY)];
		totalHeight += heightMap[getIndex(posX1, posY)];
		totalHeight += heightMap[getIndex(posX, posY1)];
		totalHeight += heightMap[getIndex(posX1, posY1)];

		computeCapacity();
		if (sediment > capacity) {
			float deposed_sediment = (sediment - capacity) * p_deposition;

			heightMap[getIndex(posX, posY)] +=
				deposed_sediment * heightMap[getIndex(posX, posY)] / totalHeight;
			heightMap[getIndex(posX, posY1)] +=
				deposed_sediment * heightMap[getIndex(posX, posY1)] / totalHeight;
			heightMap[getIndex(posX1, posY)] +=
				deposed_sediment * heightMap[getIndex(posX1, posY)] / totalHeight;
			heightMap[getIndex(posX1, posY1)] +=
				deposed_sediment * heightMap[getIndex(posX1, posY1)] / totalHeight;

			sediment -= deposed_sediment;
		}
		else {
			float eroded_sediment = min((capacity - sediment) * p_erosion, -terrain_height_diff) / 4.0f;

			heightMap[getIndex(std::floorf(pos[0]), std::floorf(pos[1]))] -= eroded_sediment;
			heightMap[getIndex(std::floorf(pos[0]), std::ceilf(pos[1]))] -= eroded_sediment;
			heightMap[getIndex(std::ceilf(pos[0]), std::floorf(pos[1]))] -= eroded_sediment;
			heightMap[getIndex(std::ceilf(pos[0]), std::ceilf(pos[1]))] -= eroded_sediment;

			sediment += eroded_sediment;
		}
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
	computeErosionStep();
	computeVelocity();
	computeWater();

	return 1;
}