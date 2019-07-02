#include "RainDrop.h"



RainDrop::RainDrop(unsigned int width, unsigned int height, std::vector<float> &heightMap) :
	width(width),
	height(height),
	heightMap(heightMap)
{
	xPos = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (float) width));
	yPos = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (float) height));
}


RainDrop::~RainDrop()
{
}

void RainDrop::computeGradient()
{
	int p_x = std::floorf(xPos);
	int p_x1 = std::ceilf(xPos);
	int p_y = std::floorf(yPos);
	int p_y1 = std::ceilf(yPos);

	float u = xPos - p_x;
	float v = yPos - p_y;
	
	gradient[0] = (heightMap[getIndex(p_x1, p_y)] - heightMap[getIndex(p_x, p_y)]) * (1 - v)
		+ (heightMap[getIndex(p_x1, p_y1)] - heightMap[getIndex(p_x, p_y1)]) * v;
	gradient[1] = (heightMap[getIndex(p_x, p_y1)] - heightMap[getIndex(p_x, p_y)]) * (1 - u)
		+ (heightMap[getIndex(p_x1, p_y1)] - heightMap[getIndex(p_x1, p_y)]) * u;
}
