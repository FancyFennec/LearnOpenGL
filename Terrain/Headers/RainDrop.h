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

	std::vector<float> pos;
	std::vector<float> oldPos;

	std::vector<float> grad = { 0, 0 };
	std::vector<float> dir = { 0, 0 };

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

	int computeStep();

private:
	float water = 1.0f; // maybe put this into the constructor

	float terrain_height;
	float terrain_height_diff;
	float vel = 0.0f;
	float sediment = 0.0f;
	float capacity = 0.0f;

	template <class T>
	float min(T a, T b) { return a < b ? a : b; };
	template <class T>
	float max(T a, T b) { return a > b ? a : b; };
	int getIndex(int i, int j) { return j * width + i; };

	void computeGradient();
	void computeDirection();
	void computePosition();
	void computeHeight();
	void computeCapacity();
	void computeErosionStep();
	void computeVelocity();
	void computeWater();
};

