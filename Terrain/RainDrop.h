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

	float p_inertia = 0.1f;
	float p_minSlope = 0.1f;
	float p_capacity = 0.1f;
	float p_deposition = 0.1f;
	float p_erosion = 0.1f;
	float p_evaporation = 0.1f;
	float p_gravity = 9.81f;

	void setInertia(float inertia) { p_inertia = inertia; };
	void setMinSlope(float minSlope) { p_minSlope = minSlope; };
	void setCapazity(float capacity) { p_capacity = capacity; };
	void setDeposition(float deposition) { p_deposition = deposition; };
	void setErosion(float erosion) { p_erosion = erosion; };
	void setEvaporation(float evaporation) { p_evaporation = evaporation; };
	void setGravity(float gravity) { p_gravity = gravity; };

private:
	int getIndex(int i, int j) { return j * width + i; };

	void computeGradient();
	void computeStep();
};

