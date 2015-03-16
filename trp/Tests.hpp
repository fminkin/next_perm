#pragma once
#include "IPermutation.hpp"

#include <vector>

class Test{
	size_t queryNumber;
	int minimalNumber;
	int maximalNumber;
	unsigned int seed;

	int rand();
public:
	Test(size_t queryNumber, int minimalNumber, int maximalNumber);

	std::vector <long long> operator()(IPermutation *testSubject);

	void srand(unsigned int seed);
};

