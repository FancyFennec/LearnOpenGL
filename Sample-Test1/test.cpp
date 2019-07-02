#include "pch.h"
#include "../Terrain/Headers/RainDrop.h"
#include "../Terrain/Headers/pNoise.h"
#include "../Terrain/Headers/Terrain.h"

TEST(TestCaseName, TestName) {
	
	unsigned int width = 10;
	unsigned int height = 10;

	Terrain terrain(10, 10);
	terrain.generateTerrain(1, 1, 1, 0);

	RainDrop drop(width, height, terrain.heightMap);
	std::cout << drop.computeStep() << '\n';

  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}