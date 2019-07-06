// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>

void sortVector(std::vector<int>& indices)
{
	if (indices[0] > indices[2]) {
		std::iter_swap(indices.begin(), indices.begin() + 2);
	}
	if (indices[1] > indices[3]) {
		std::iter_swap(indices.begin() + 1, indices.begin() + 3);
	}
	if (indices[0] > indices[1]) {
		std::iter_swap(indices.begin(), indices.begin() + 1);
	}
	if (indices[2] > indices[3]) {
		std::iter_swap(indices.begin() + 2, indices.begin() + 3);
	}
}

int main()
{
	std::vector<int> indices = { 23, 1, 1, 22 };
	sortVector(indices);

	for (int a : indices) {
		std::cout << '\n' << a;
	}
	
}
