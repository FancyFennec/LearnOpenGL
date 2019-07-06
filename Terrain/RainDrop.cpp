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

	computeCapacity();

	if (terrain_height_diff > 0 || sediment > capacity) {

		std::vector<int> indices = { posX, posY, posX1, posY, posX, posY1, posX1, posY1 };
		sortIndices(indices);

		float heightDiff = 0.0f;
		float deposed_sediment = terrain_height_diff > 0 ? min(terrain_height_diff, sediment) : (sediment - capacity) * p_deposition;
		sediment -= deposed_sediment;

		if (heightMap[getIndex(indices[0], indices[1])] + deposed_sediment <=
			heightMap[getIndex(indices[2], indices[3])]){

			heightMap[getIndex(indices[0], indices[1])] += deposed_sediment;

		} else{
			heightDiff = heightMap[getIndex(indices[2], indices[3])] - heightMap[getIndex(indices[0], indices[1])];
			heightMap[getIndex(indices[0], indices[1])] += heightDiff;
			deposed_sediment -= heightDiff;

			if (heightMap[getIndex(indices[2], indices[3])] + deposed_sediment / 2.0f <=
				heightMap[getIndex(indices[4], indices[5])]) {

				heightMap[getIndex(indices[0], indices[1])] += deposed_sediment / 2.0f;
				heightMap[getIndex(indices[2], indices[3])] += deposed_sediment / 2.0f;

			} else {
				heightDiff = heightMap[getIndex(indices[4], indices[5])] - heightMap[getIndex(indices[2], indices[3])];
				heightMap[getIndex(indices[0], indices[1])] += heightDiff;
				heightMap[getIndex(indices[2], indices[3])] += heightDiff;
				deposed_sediment -= heightDiff * 2;

				if (heightMap[getIndex(indices[4], indices[5])] + deposed_sediment / 3.0f <=
					heightMap[getIndex(indices[6], indices[7])]) {

					heightMap[getIndex(indices[0], indices[1])] += deposed_sediment / 3.0f;
					heightMap[getIndex(indices[2], indices[3])] += deposed_sediment / 3.0f;
					heightMap[getIndex(indices[4], indices[5])] += deposed_sediment / 3.0f;

				}
				else {
					heightDiff = heightMap[getIndex(indices[6], indices[7])] - heightMap[getIndex(indices[4], indices[5])];
					heightMap[getIndex(indices[0], indices[1])] += heightDiff;
					heightMap[getIndex(indices[2], indices[3])] += heightDiff;
					heightMap[getIndex(indices[4], indices[5])] += heightDiff;
					deposed_sediment -= heightDiff * 3;

					if (deposed_sediment > 0) {
						heightMap[getIndex(indices[0], indices[1])] += deposed_sediment / 4.0f;
						heightMap[getIndex(indices[2], indices[3])] += deposed_sediment / 4.0f;
						heightMap[getIndex(indices[4], indices[5])] += deposed_sediment / 4.0f;
						heightMap[getIndex(indices[6], indices[7])] += deposed_sediment / 4.0f;
					}
				}
			}
		}
	} else {

		float d1 = max(0.0f, 1.41f - norm(pos[0] - posX, pos[1] - posY));
		float d2 = max(0.0f, 1.41f - norm(pos[0] - posX1, pos[1] - posY));
		float d3 = max(0.0f, 1.41f - norm(pos[0] - posX, pos[1] - posY1));
		float d4 = max(0.0f, 1.41f - norm(pos[0] - posX1, pos[1] - posY1));

		float totalWeight = d1 + d2 + d3 + d4;

		float w1 = d1 / totalWeight;
		float w2 = d2 / totalWeight;
		float w3 = d3 / totalWeight;
		float w4 = d4 / totalWeight;

		float eroded_sediment = min((capacity - sediment) * p_erosion, -terrain_height_diff);

		heightMap[getIndex(posX, posY)] -= eroded_sediment * w1;
		heightMap[getIndex(posX1, posY)] -= eroded_sediment * w2;
		heightMap[getIndex(posX, posY1)] -= eroded_sediment * w3;
		heightMap[getIndex(posX1, posY1)] -= eroded_sediment * w4;

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
	computeErosionStep();
	computeVelocity();
	computeWater();

	return 1;
}