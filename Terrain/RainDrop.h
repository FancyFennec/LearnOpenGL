#pragma once

#include <vector>
#include <random>
class RainDrop
{
public:
	RainDrop(unsigned int width, unsigned int height, std::vector<float> &heightMap);
	~RainDrop();

	unsigned int width, height;
	std::vector<float> &heightMap;

	float xPos;
	float yPos;
	std::vector<float> gradient = { 0, 0 };

private:
	int getIndex(int i, int j) { return j * width + i; };

	void computeGradient();
	void computeStep();
};

